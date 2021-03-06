#include <gdal_priv.h>

#include <iomanip>
#include <iostream>
#include <vector>
#define BYTE float
#define nodata -10000
using namespace std;
struct GeoPoint {
  GeoPoint(double x, double y, double elev)
      : Xgeo(x), Ygeo(y), elevation(elev) {}
  double Xgeo;
  double Ygeo;
  double elevation;
};
struct ProjectionDataset {
  double adfULCoord[2];                  //左上角坐标
  double adfLRCoord[2];                  //右下角坐标
  double step;                           //分辨率
  const char* pszProj;                   //投影信息
  vector<vector<GeoPoint> > RasterData;  //数据集
};
/*
    @brief 计算图像行列号在给定坐标系下对应的地理坐标
    @param x                行号
    @param y                列号
    @param coords           返回的地理坐标
    @param transform        变换的六参数
*/
void toGeoCoord(int x, int y, double* coords, double* transform) {
  coords[0] = transform[0] + x * transform[1] + y * transform[2];
  coords[1] = transform[3] + x * transform[4] + y * transform[5];
}
int readGeoRaster(const char* file_path_name, ProjectionDataset& Data) {
  GDALAllRegister();
  GDALDataset* poDataset;
  poDataset = (GDALDataset*)GDALOpen(file_path_name, GA_ReadOnly);
  if (poDataset == NULL) {
    printf("fail in open files: (%s)", file_path_name);
    return false;
  }
  BYTE* pafScanblock1;
  //获取图像波段
  GDALRasterBand* poBand1;
  poBand1 = poDataset->GetRasterBand(1);

  //获取图像的尺寸
  int nImgSizeX = poDataset->GetRasterXSize();
  int nImgSizeY = poDataset->GetRasterYSize();
  cout << nImgSizeX << " " << nImgSizeY << endl;
  //获取坐标变换系数
  double trans[6];
  CPLErr aaa = poDataset->GetGeoTransform(trans);  //读取图像高程数据
  float Xgeo, Ygeo;
  GDALDataType gdal_data_type =
      poDataset->GetRasterBand(1)->GetRasterDataType();
  pafScanblock1 = (BYTE*)CPLMalloc(sizeof(BYTE) * (nImgSizeX) * (nImgSizeY)*3);
  poBand1->RasterIO(GF_Read, 0, 0, nImgSizeX, nImgSizeY, pafScanblock1,
                    nImgSizeX, nImgSizeY, GDT_Float32, 0, 0);
  toGeoCoord(0, 0, Data.adfULCoord, trans);
  toGeoCoord(nImgSizeX - 1, nImgSizeY - 1, Data.adfLRCoord, trans);
  Data.pszProj = poDataset->GetProjectionRef();  // 获得WKT形式的投影信息
  Data.RasterData = vector<vector<GeoPoint> >(nImgSizeX);
  for (int i = 0; i < nImgSizeX; i++) {
    for (int j = 0; j < nImgSizeY; j++) {
      float elevation = *pafScanblock1;
      Xgeo = trans[0] + i * trans[1] + j * trans[2];
      Ygeo = trans[3] + i * trans[4] + j * trans[5];
      Data.RasterData[i].push_back(GeoPoint(Xgeo, Ygeo, elevation));
    }
  }
  Data.step = fabs(Data.RasterData[0][1].Ygeo - Data.RasterData[0][0].Ygeo);
  delete poDataset;  // 关闭数据集
  return true;
}
// int main() {
//   cout << "hello world" << endl;
//   ProjectionDataset Map;
//   cout << readGeoRaster("dem.tif", Map) << endl;
//   cout << Map.step << endl;
//   cout << Map.adfLRCoord[0] << " " << Map.adfLRCoord[1] << endl;
//   for (int i = 0; i < 50; i++) {
//     for (int j = 0; j < 50; j++) {
//       GeoPoint p = Map.RasterData[i][j];
//       cout << p.Xgeo << " " << p.Ygeo << " " << p.elevation << endl;
//     }
//   }
//   return 0;
// }
