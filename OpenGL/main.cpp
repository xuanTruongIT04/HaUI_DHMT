#pragma region 
#include "Angel.h"
#include <cstdlib>

typedef vec4 point4;
typedef vec4 color4;
using namespace std;
#pragma endregion Khai báo thư viện và định nghĩa kiểu dữ liệu

#pragma region 
GLuint program;
GLuint model_loc;
GLuint projection_loc;
GLuint view_loc;
#pragma endregion Các biến kết hợp vshader dùng để chạy chương trình, NO LOGIC

#pragma region
mat4 wolrd_matrix;
mat4 projection;
mat4 view;
#pragma endregion Các biến ma trận model, projection, view, symbol

#pragma region
// Số các đỉnh của các tam giác trong hình lập phương
const int soDinhLapPhuong = 36;
// Số các đỉnh của các tam giác trong hình trụ (độ chính xác 8 mặt)
// Mỗi mặt ngang là 1 hcn = 2 tam giác => 6 đỉnh, tổng 8 mặt = 6 * 8
// 2 đáy, mỗi đáy 8 tam giác = 3 * 8 * 2 đáy
// 6 * 8 + 3 * 8 * 2 = 96
const int soDinhHinhTru = 96;
// 4 * 6 * 8 = 192
const int soDinhHinhTron = 192;

// Danh sách các đỉnh của các tam giác cần vẽ
point4 arrDinhTamGiac[soDinhLapPhuong + soDinhHinhTru + soDinhHinhTron];
// Danh sách các màu tương ứng cho các đỉnh trên
color4 arrMauCacDinh[soDinhLapPhuong + soDinhHinhTru + soDinhHinhTron];
//Danh sách các vector pháp tuyến ứng với mỗi đỉnh
vec3 arrVecPhapTuyen[soDinhLapPhuong + soDinhHinhTru + soDinhHinhTron];


// Danh sách 8 đỉnh của hình lập phương
point4 cacDinhLapPhuong[8];
// Danh sách các màu cho 8 đỉnh hình lập phương
color4 cacMauHinhLapPhuong[8];

// Danh sách các đỉnh của hình trụ
// 8 cạnh bên, mỗi cạnh 2 đỉnh, 2 tâm tại 2 đáy
// 2 * 8 + 2 = 18
point4 cacDinhHinhTru[18];
// Danh sách các màu cho 10 mặt hình trụ
color4 cacMauHinhTru[10];

// Hình tròn
point4 cacDinhHinhTron[5][8];
color4 cacMauHinhTron[10];
#pragma endregion Các biến tính toán trong bộ nhớ

#pragma region
// Vị trí ánh sáng
point4 viTriAnhSang(0.0, 10.0, 10.0, 1.0);
// Màu gốc ánh sáng
color4 mauAnhSang(1, 1, 1, 1.0);
// Màu khuếch tán
color4 mauAnhSangKhuechTan(1.0, 1.0, 1.0, 1.0);
// Màu gương phản xạ lại
color4 mauAnhSangPhanXaGuong(1.0, 1.0, 1.0, 1.0);

// Màu gốc vật liệu
color4 mauVatLieu;
// Màu khuếch tán vật liệu
color4 mauVatLieuKhuechTan;
// Màu phản xạ gương vật liệu
color4 mauVatLieuPhanXaGuong;
// Độ bóng vật liệu
float doBongVatLieu = 1000.0;

// Màu được trộn từ 2 màu gốc
color4 mauGocTron;
// Màu khuếch tán được trộn từ 2 màu gốc
color4 mauKhuechTanTron;
// Màu phản xạ gương được trộn từ 2 màu gốc
color4 mauPhanXaGuongTron;
#pragma  endregion Các biến ánh sáng, vật liệu

#pragma region 
GLfloat cam_Eye[3] = { 0, 5, 6 };
float cam_Rotation[3] = { 0,90,90 };
GLfloat dr = 5;
vec4 eye(0, 5, -5, 1);
vec4 at(0, 0, 0, 1);
vec4 up(0, 1, 0, 1);

bool che_do_chi_tiet = false;
#pragma endregion Các biến hành động

#pragma region 
// Gán toạ độ điểm và giá trị màu cho HLP
void TinhDiemVaMauLapPhuong()
{
	// Gán giá trị tọa độ vị trí cho các đỉnh của hình lập phương
	cacDinhLapPhuong[0] = point4(-0.5, -0.5, 0.5, 1.0);
	cacDinhLapPhuong[1] = point4(-0.5, 0.5, 0.5, 1.0);
	cacDinhLapPhuong[2] = point4(0.5, 0.5, 0.5, 1.0);
	cacDinhLapPhuong[3] = point4(0.5, -0.5, 0.5, 1.0);
	cacDinhLapPhuong[4] = point4(-0.5, -0.5, -0.5, 1.0);
	cacDinhLapPhuong[5] = point4(-0.5, 0.5, -0.5, 1.0);
	cacDinhLapPhuong[6] = point4(0.5, 0.5, -0.5, 1.0);
	cacDinhLapPhuong[7] = point4(0.5, -0.5, -0.5, 1.0);

	// Gán giá trị màu sắc cho các đỉnh của hình lập phương	
	cacMauHinhLapPhuong[0] = color4(0.0, 0.0, 0.0, 1.0); // đen
	cacMauHinhLapPhuong[1] = color4(1.0, 0.0, 0.0, 1.0); // đỏ
	cacMauHinhLapPhuong[2] = color4(1.0, 1.0, 0.0, 1.0); // vàng
	cacMauHinhLapPhuong[3] = color4(0.0, 1.0, 0.0, 1.0); // xanh lá
	cacMauHinhLapPhuong[4] = color4(0.0, 0.0, 1.0, 1.0); // xanh lam
	cacMauHinhLapPhuong[5] = color4(1.0, 0.0, 1.0, 1.0); // tím
	cacMauHinhLapPhuong[6] = color4(1.0, 0.5, 0.0, 1.0); // cam
	cacMauHinhLapPhuong[7] = color4(0.0, 1.0, 1.0, 1.0); // xanh lơ
}
// Gán toạ độ điểm và giá trị màu cho hình trụ
void TinhDiemVaMauHinhTru()
{
	float tren = 0.5;
	float duoi = -tren;
	// Tâm trên
	cacDinhHinhTru[0] = point4(0, tren, 0, 1);

	// For để gán cho các đỉnh cạnh bên
	for (int i = 1; i <= 8; i++)
	{
		float banKinh = 0.5;
		float rad = (i - 1) * 45.0 / 360 * 2 * M_PI;
		// Tính x va z
		float x = banKinh * cosf(rad);
		float z = banKinh * sinf(rad);
		cacDinhHinhTru[i * 2 - 1] = point4(x, tren, z, 1);
		cacDinhHinhTru[i * 2] = point4(x, duoi, z, 1);
	}

	// Tâm dưới
	cacDinhHinhTru[17] = point4(0, duoi, 0, 1);

	cacMauHinhTru[0] = color4(1.0, 0.0, 0.0, 1.0); // đỏ
	cacMauHinhTru[1] = color4(1.0, 1.0, 0.0, 1.0); // vàng
	cacMauHinhTru[2] = color4(0.0, 1.0, 0.0, 1.0); // xanh lá
	cacMauHinhTru[3] = color4(0.0, 0.0, 1.0, 1.0); // xanh lam
	cacMauHinhTru[4] = color4(1.0, 0.0, 1.0, 1.0); // tím
	cacMauHinhTru[5] = color4(1.0, 0.0, 0.0, 1.0); // đỏ
	cacMauHinhTru[6] = color4(1.0, 1.0, 0.0, 1.0); // vàng
	cacMauHinhTru[7] = color4(0.0, 1.0, 0.0, 1.0); // xanh lá
	cacMauHinhTru[8] = color4(0.0, 0.0, 1.0, 1.0); // xanh lam
	cacMauHinhTru[9] = color4(1.0, 0.0, 1.0, 1.0); // tím
}
// Gán toạ độ điểm và giá trị màu cho hình tròn
void TinhDiemVaMauHinhTron()
{
	float x, y, z;
	const float r = 0.5;
	const float cv45torad = 45.0 / 360 * 2 * M_PI;
	for (int i = 0; i <= 4; i++)
	{
		y = -r * cosf(i * cv45torad);
		float bk = r * sinf(i * cv45torad);
		//Tính y
		for (int j = 0; j <= 7; j++)
		{
			// Tính x,z
			x = bk * cosf(j * cv45torad);
			z = -bk * sinf(j * cv45torad);
			cacDinhHinhTron[i][j] = point4(x, y, z, 1);
		}
	}

	cacMauHinhTron[0] = color4(1.0, 0.0, 0.0, 1.0); // đỏ
	cacMauHinhTron[1] = color4(1.0, 1.0, 0.0, 1.0); // vàng
	cacMauHinhTron[2] = color4(0.0, 1.0, 0.0, 1.0); // xanh lá
	cacMauHinhTron[3] = color4(0.0, 0.0, 1.0, 1.0); // xanh lam
	cacMauHinhTron[4] = color4(1.0, 0.0, 1.0, 1.0); // tím
	cacMauHinhTron[5] = color4(1.0, 0.0, 0.0, 1.0); // đỏ
	cacMauHinhTron[6] = color4(1.0, 1.0, 0.0, 1.0); // vàng
	cacMauHinhTron[7] = color4(0.0, 1.0, 0.0, 1.0); // xanh lá
	cacMauHinhTron[8] = color4(0.0, 0.0, 1.0, 1.0); // xanh lam
	cacMauHinhTron[9] = color4(1.0, 0.0, 1.0, 1.0); // tím
}

// Start HLP = 0
// Start Hình trụ = 0 + 36
// Start Hình tròn = 36 + 96 = 132
int Index = 0;
// Tạo một mặt hình lập phương (đưa đỉnh, pháp tuyến, màu vào mảng thứ tự cho việc chuyển đến bộ nhớ, )
void TaoMotMatHLP(int a, int b, int c, int d)
{
	vec4 u = cacDinhLapPhuong[b] - cacDinhLapPhuong[a];
	vec4 v = cacDinhLapPhuong[c] - cacDinhLapPhuong[b];
	vec3 normal = normalize(cross(u, v));

	arrVecPhapTuyen[Index] = normal; arrMauCacDinh[Index] = cacMauHinhLapPhuong[a]; arrDinhTamGiac[Index] = cacDinhLapPhuong[a]; Index++;
	arrVecPhapTuyen[Index] = normal; arrMauCacDinh[Index] = cacMauHinhLapPhuong[a]; arrDinhTamGiac[Index] = cacDinhLapPhuong[b]; Index++;
	arrVecPhapTuyen[Index] = normal; arrMauCacDinh[Index] = cacMauHinhLapPhuong[a]; arrDinhTamGiac[Index] = cacDinhLapPhuong[c]; Index++;
	arrVecPhapTuyen[Index] = normal; arrMauCacDinh[Index] = cacMauHinhLapPhuong[a]; arrDinhTamGiac[Index] = cacDinhLapPhuong[a]; Index++;
	arrVecPhapTuyen[Index] = normal; arrMauCacDinh[Index] = cacMauHinhLapPhuong[a]; arrDinhTamGiac[Index] = cacDinhLapPhuong[c]; Index++;
	arrVecPhapTuyen[Index] = normal; arrMauCacDinh[Index] = cacMauHinhLapPhuong[a]; arrDinhTamGiac[Index] = cacDinhLapPhuong[d]; Index++;
}
// Dùng hàm bên trên để đưa vào mảng
void TaoHinhVuong()  /* Sinh ra 12 tam giác: 36 đỉnh, 36 màu*/
{
	TaoMotMatHLP(1, 0, 3, 2);
	TaoMotMatHLP(2, 3, 7, 6);
	TaoMotMatHLP(3, 0, 4, 7);
	TaoMotMatHLP(6, 5, 1, 2);
	TaoMotMatHLP(4, 5, 6, 7);
	TaoMotMatHLP(5, 4, 0, 1);
}


// Hình trụ
void TaoMatBenHTru(int a, int b, int c, int d, int mau)
{
	vec4 u = cacDinhHinhTru[b] - cacDinhHinhTru[a];
	vec4 v = cacDinhHinhTru[c] - cacDinhHinhTru[b];
	vec3 phapTuyen = normalize(cross(u, v));

	// a -> b -> c -> a -> c -> d
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTru[mau]; arrDinhTamGiac[Index] = cacDinhHinhTru[a]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTru[mau]; arrDinhTamGiac[Index] = cacDinhHinhTru[b]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTru[mau]; arrDinhTamGiac[Index] = cacDinhHinhTru[c]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTru[mau]; arrDinhTamGiac[Index] = cacDinhHinhTru[a]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTru[mau]; arrDinhTamGiac[Index] = cacDinhHinhTru[c]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTru[mau]; arrDinhTamGiac[Index] = cacDinhHinhTru[d]; Index++;
}

void TaoTamGiacDay(int a, int b, int c, int mau)
{
	vec4 u = cacDinhHinhTru[b] - cacDinhHinhTru[a];
	vec4 v = cacDinhHinhTru[c] - cacDinhHinhTru[b];
	vec3 phapTuyen = normalize(cross(u, v));

	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTru[mau]; arrDinhTamGiac[Index] = cacDinhHinhTru[a]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTru[mau]; arrDinhTamGiac[Index] = cacDinhHinhTru[b]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTru[mau]; arrDinhTamGiac[Index] = cacDinhHinhTru[c]; Index++;
}

void TaoHTru()
{
	// Mỗi lệnh 6 điểm, 8 lệnh 48 điểm
	TaoMatBenHTru(2, 1, 3, 4, 0);
	TaoMatBenHTru(4, 3, 5, 6, 1);
	TaoMatBenHTru(6, 5, 7, 8, 2);
	TaoMatBenHTru(8, 7, 9, 10, 3);
	TaoMatBenHTru(10, 9, 11, 12, 4);
	TaoMatBenHTru(12, 11, 13, 14, 5);
	TaoMatBenHTru(14, 13, 15, 16, 6);
	TaoMatBenHTru(16, 15, 1, 2, 7);
	// Kết thúc tại điểm 2

	// Mỗi lệnh 3 điểm, 8 lệnh 24 điểm
	// Bắt đầu tại điểm 2
	TaoTamGiacDay(2, 17, 16, 9);
	TaoTamGiacDay(16, 17, 14, 9);
	TaoTamGiacDay(14, 17, 12, 9);
	TaoTamGiacDay(12, 17, 10, 9);
	TaoTamGiacDay(10, 17, 8, 9);
	TaoTamGiacDay(8, 17, 6, 9);
	TaoTamGiacDay(6, 17, 4, 9);
	TaoTamGiacDay(4, 17, 2, 9);

	// Mỗi lệnh 3 điểm, 8 lệnh 24 điểm
	TaoTamGiacDay(1, 0, 3, 8);
	TaoTamGiacDay(3, 0, 5, 8);
	TaoTamGiacDay(5, 0, 7, 8);
	TaoTamGiacDay(7, 0, 9, 8);
	TaoTamGiacDay(9, 0, 11, 8);
	TaoTamGiacDay(11, 0, 13, 8);
	TaoTamGiacDay(13, 0, 15, 8);
	TaoTamGiacDay(15, 0, 1, 8);

	//Tổng 48 + 24 + 24 = 96 = biến soDinhHinhTru
}


// Hình tròn
void TaoMatBenHTron(int i, int j, int i1, int j1, int mau)
{
	vec3 phapTuyen;
	vec4 u;
	vec4 v;
	if (i == 0)
	{
		u = cacDinhHinhTron[i1][j1] - cacDinhHinhTron[i][j];
		v = cacDinhHinhTron[i1][j] - cacDinhHinhTron[i][j];
		phapTuyen = normalize(cross(u, v));
	}
	else
	{
		u = cacDinhHinhTron[i][j1] - cacDinhHinhTron[i][j];
		v = cacDinhHinhTron[i1][j1] - cacDinhHinhTron[i][j1];
		phapTuyen = normalize(cross(u, v));
	}
	// a -> b -> c -> a -> c -> d
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTron[mau]; arrDinhTamGiac[Index] = cacDinhHinhTron[i][j]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTron[mau]; arrDinhTamGiac[Index] = cacDinhHinhTron[i][j1]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTron[mau]; arrDinhTamGiac[Index] = cacDinhHinhTron[i1][j1]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTron[mau]; arrDinhTamGiac[Index] = cacDinhHinhTron[i][j]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTron[mau]; arrDinhTamGiac[Index] = cacDinhHinhTron[i1][j1]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTron[mau]; arrDinhTamGiac[Index] = cacDinhHinhTron[i1][j]; Index++;
}

void TaoHTron()
{
	for (int j = 0; j <= 6; j++)
	{
		for (int i = 0; i <= 3; i++)
		{
			TaoMatBenHTron(i, j, i + 1, j + 1, 4);
		}
	}

	for (int i = 0; i <= 3; i++)
	{
		TaoMatBenHTron(i, 7, i + 1, 0, 3);
	}
}


void GenerateCacHinh(void)
{
	TinhDiemVaMauLapPhuong();
	TaoHinhVuong();

	TinhDiemVaMauHinhTru();
	TaoHTru();

	TinhDiemVaMauHinhTron();
	TaoHTron();
}

#pragma endregion Xử lí Generate các hình

#pragma region
// Hàm đổi _param, ví dụ 255 => 255/255 = 1
float RGBConvert(float _param)
{
	return _param / 255;
}
color4 RGBtoColor(int _R, int _G, int _B)
{
	return color4(RGBConvert(_R), RGBConvert(_G), RGBConvert(_B), 1.0);
}
color4 RGBtoColor(int _R, int _G, int _B, int _A)
{
	return color4(RGBConvert(_R), RGBConvert(_G), RGBConvert(_B), RGBConvert(_A));
}
float DEGtoRAD(float DEG)
{
	return DEG / 360 * 2 * M_PI;
}
void TronMau()
{
	mauGocTron = mauAnhSang * mauVatLieu;
	mauKhuechTanTron = mauAnhSangKhuechTan * mauVatLieuKhuechTan;
	mauPhanXaGuongTron = mauAnhSangPhanXaGuong * mauVatLieuPhanXaGuong;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, mauGocTron);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, mauKhuechTanTron);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, mauPhanXaGuongTron);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, viTriAnhSang);
	glUniform1f(glGetUniformLocation(program, "Shininess"), doBongVatLieu);
}
// Xem hình để biết về hàm này
void TaoVatLieu(color4 mauGoc, color4 mauPhanXa, color4 mauPXGuong, float doBong)
{
	mauVatLieu = mauGoc;
	mauVatLieuPhanXaGuong = mauPXGuong;
	mauVatLieuKhuechTan = mauPhanXa;
	doBongVatLieu = doBong;
	TronMau();
}
// Hàm vẽ hình lập phương tâm O,  với ma trận _matrixPhanCap * _symbol
void VeHinhLapPhuong(mat4 _symbol, mat4 _matrixPhanCap)
{
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, wolrd_matrix * _matrixPhanCap * _symbol);
	glDrawArrays(GL_TRIANGLES, 0, soDinhLapPhuong);    /*Vẽ các tam giác*/
}
// Hàm vẽ hình trụ tâm O, bán kính đáy 0.5, chiều cao 1 với ma trận _matrix * _symbol
void VeHinhTru(mat4 _symbol, mat4 _matrix)
{
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, wolrd_matrix * _matrix * _symbol);
	glDrawArrays(GL_TRIANGLES, 36, soDinhHinhTru);    /*Vẽ các tam giác*/
}
// Hàm vẽ hình tròn tâm O, bán kính 0.5 với ma trận _matrix * _symbol
void VeHinhTron(mat4 _symbol, mat4 _matrix)
{
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, wolrd_matrix * _matrix * _symbol);
	glDrawArrays(GL_TRIANGLES, 132, soDinhHinhTron);    /*Vẽ các tam giác*/
}
#pragma endregion Các hàm tạo sẵn

#pragma region 
void KhoiTaoBoDemGPU(void)
{
	// Tạo một VAO - vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Tạo và khởi tạo một buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(arrDinhTamGiac) + sizeof(arrMauCacDinh) + sizeof(arrVecPhapTuyen), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(arrDinhTamGiac), arrDinhTamGiac);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(arrDinhTamGiac), sizeof(arrMauCacDinh), arrMauCacDinh);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(arrDinhTamGiac) + sizeof(arrMauCacDinh), sizeof(arrVecPhapTuyen), arrVecPhapTuyen);
}

void SetupShader(void)
{
	// Nạp các shader và sử dụng chương trình shader
	program = InitShader("vshader1.glsl", "fshader1.glsl");   // hàm InitShader khai báo trong Angel.h
	glUseProgram(program);

	// Khởi tạo thuộc tính vị trí đỉnh từ vertex shader
	GLuint loc_vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc_vPosition);
	glVertexAttribPointer(loc_vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint loc_vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc_vColor);
	glVertexAttribPointer(loc_vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(arrDinhTamGiac)));

	GLuint loc_vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(loc_vNormal);
	glVertexAttribPointer(loc_vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(arrDinhTamGiac) + sizeof(arrMauCacDinh)));

	model_loc = glGetUniformLocation(program, "Model");
	projection_loc = glGetUniformLocation(program, "Projection");
	view_loc = glGetUniformLocation(program, "View");

	glEnable(GL_DEPTH_TEST);
	glClearColor(33.0 / 255, 114.0 / 255, 160.0 / 255, 1.0);        /* Thiết lập màu trắng là màu xóa màn hình*/
}
#pragma endregion Tạo GPU buffer và setup shader

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///																											  ///
///        ĐỂ VẼ MỘT HÌNH HÃY TẠO VẬT LIỆU VÀ DÙNG 1 TRONG 3 HÀM VẼ HÌNH, XEM MẪU BÀN TRƯỚC KHI LÀM           ///
///																											  ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region CarB
mat4 matrixPhanCapCarB;
color4 CarB_vl;
color4 CarB_kt;
color4 CarB_pxg;
mat4 CarB_symbol;

// Biến dùng để di chuyển
float CarB_DiChuyenX = 0;
float CarB_DiChuyenZ = 0;
float CarB_XeXoay = 0;
float CarB_BanhTruocXoay = 0;
float CarB_BanhXeXoay = 0;
float CarB_MoCuaXeTrai = 0;
float CarB_MoCuaXePhai = 0;

void CarB_BanhSau()
{
	CarB_vl = RGBtoColor(30, 20, 30);
	CarB_kt = RGBtoColor(35, 30, 40);
	CarB_pxg = RGBtoColor(40, 40, 50);
	TaoVatLieu(CarB_vl, CarB_kt, CarB_pxg, 1000);

	// Bánh trái sau
	CarB_symbol = Translate(0, 0, -1.4) * Scale(1, 1, 0.3) * RotateX(90);
	VeHinhTru(CarB_symbol, matrixPhanCapCarB);

	// Bánh phải sau
	CarB_symbol = Translate(0, 0, 1.4) * Scale(1, 1, 0.3) * RotateX(90);
	VeHinhTru(CarB_symbol, matrixPhanCapCarB);

	//Trục bánh xe
	CarB_vl = RGBtoColor(82, 135, 190);
	CarB_kt = RGBtoColor(75, 120, 180);
	CarB_pxg = RGBtoColor(70, 110, 170);
	TaoVatLieu(CarB_vl, CarB_kt, CarB_pxg, 1000);
	CarB_symbol = Scale(0.3, 0.3, 3.2) * RotateX(90);
	VeHinhTru(CarB_symbol, matrixPhanCapCarB);
}

void CarB_BanhTruoc()
{
	CarB_vl = RGBtoColor(30, 20, 30);
	CarB_kt = RGBtoColor(35, 30, 40);
	CarB_pxg = RGBtoColor(40, 40, 50);
	TaoVatLieu(CarB_vl, CarB_kt, CarB_pxg, 1000);

	CarB_symbol = Scale(1, 1, 0.3) * RotateX(90);
	VeHinhTru(CarB_symbol, matrixPhanCapCarB);

	//Trục bánh xe
	CarB_vl = RGBtoColor(82, 135, 190);
	CarB_kt = RGBtoColor(75, 120, 180);
	CarB_pxg = RGBtoColor(70, 110, 170);
	TaoVatLieu(CarB_vl, CarB_kt, CarB_pxg, 1000);
	CarB_symbol = Scale(0.3, 0.3, 0.36) * RotateX(90);
	VeHinhTru(CarB_symbol, matrixPhanCapCarB);
}

void CarB_ThanXe()
{
	CarB_vl = RGBtoColor(10, 100, 220);
	CarB_kt = RGBtoColor(30, 120, 200);
	CarB_pxg = RGBtoColor(50, 130, 180);
	TaoVatLieu(CarB_vl, CarB_kt, CarB_pxg, 1000);

	//Mặt sau đầu xe
	CarB_symbol = Translate(2, 0, 1.4) * Scale(0.1, 2, 0.2);
	VeHinhLapPhuong(CarB_symbol, matrixPhanCapCarB);

	//Mặt sau đầu xe
	CarB_symbol = Translate(2, 0, -1.4) * Scale(0.1, 2, 0.2);
	VeHinhLapPhuong(CarB_symbol, matrixPhanCapCarB);

	//Mặt trước đầu xe
	CarB_symbol = Translate(4.65, -0.4, 0) * Scale(1.5, 1.1, 3);
	VeHinhLapPhuong(CarB_symbol, matrixPhanCapCarB);

	//Đuổi xe
	CarB_symbol = Translate(-0.8, -0.35, 0) * Scale(1.8, 1.2, 3);
	VeHinhLapPhuong(CarB_symbol, matrixPhanCapCarB);

	//Mặt trên đầu xe
	CarB_symbol = Translate(1.85, 0.95, 0) * Scale(2.8, 0.1, 3);
	VeHinhLapPhuong(CarB_symbol, matrixPhanCapCarB);

	//Thanh chéo trước đầu xe bên trái
	CarB_symbol = Translate(3.53, 0.54, -1.4) * RotateZ(-45) * Scale(1.2, 0.1, 0.2);
	VeHinhLapPhuong(CarB_symbol, matrixPhanCapCarB);

	//Thanh chéo trước đầu xe bên trái
	CarB_symbol = Translate(3.53, 0.54, 1.4) * RotateZ(-45) * Scale(1.2, 0.1, 0.2);
	VeHinhLapPhuong(CarB_symbol, matrixPhanCapCarB);

	//Thanh chéo sau đầu xe bên trái
	CarB_symbol = Translate(0.2, 0.6, -1.4) * RotateZ(50) * Scale(1, 0.1, 0.2);
	VeHinhLapPhuong(CarB_symbol, matrixPhanCapCarB);

	//Thanh chéo sau đầu xe bên trái
	CarB_symbol = Translate(0.2, 0.6, 1.4) * RotateZ(50) * Scale(1, 0.1, 0.2);
	VeHinhLapPhuong(CarB_symbol, matrixPhanCapCarB);

	CarB_vl = RGBtoColor(200, 80, 80);
	CarB_kt = RGBtoColor(190, 90, 100);
	CarB_pxg = RGBtoColor(180, 100, 120);
	TaoVatLieu(CarB_vl, CarB_kt, CarB_pxg, 1000);

	//Mặt dưới đầu xe
	CarB_symbol = Translate(1.8, -0.95, 0) * Scale(7.2, 0.1, 2.98);
	VeHinhLapPhuong(CarB_symbol, matrixPhanCapCarB);

	// Hình trụ đầu xe
	CarB_symbol = Translate(5.4, -0.65, 0) * RotateX(90) * Scale(0.6, 2.95, 0.6);
	VeHinhTru(CarB_symbol, matrixPhanCapCarB);


	//Ghế truoc
	CarB_vl = RGBtoColor(129, 156, 96);
	CarB_kt = RGBtoColor(129, 156, 96);
	CarB_pxg = RGBtoColor(129, 156, 96);
	TaoVatLieu(CarB_vl, CarB_kt, CarB_pxg, 1000);
	CarB_symbol = Translate(2.6, -0.8, 0.8) * Scale(0.8, 0.3, 1);// ghế
	VeHinhLapPhuong(CarB_symbol, matrixPhanCapCarB);
	CarB_symbol = Translate(2.25, -0.15, 0.8) * Scale(0.1, 1.2, 1) * RotateZ(0);// tựa
	VeHinhLapPhuong(CarB_symbol, matrixPhanCapCarB);

	CarB_symbol = Translate(2.6, -0.8, -0.8) * Scale(0.8, 0.3, 1);// ghế
	VeHinhLapPhuong(CarB_symbol, matrixPhanCapCarB);
	CarB_symbol = Translate(2.25, -0.15, -0.8) * Scale(0.1, 1.2, 1) * RotateZ(0);// tựa
	VeHinhLapPhuong(CarB_symbol, matrixPhanCapCarB);

	//Ghế sau
	CarB_symbol = Translate(0.8, -0.8, 0.8) * Scale(0.8, 0.3, 1);// ghế
	VeHinhLapPhuong(CarB_symbol, matrixPhanCapCarB);
	CarB_symbol = Translate(0.45, -0.15, 0.8) * Scale(0.1, 1.2, 1) * RotateZ(0);// tựa
	VeHinhLapPhuong(CarB_symbol, matrixPhanCapCarB);

	CarB_symbol = Translate(0.8, -0.8, -0.8) * Scale(0.8, 0.3, 1);// ghế
	VeHinhLapPhuong(CarB_symbol, matrixPhanCapCarB);
	CarB_symbol = Translate(0.45, -0.15, -0.8) * Scale(0.1, 1.2, 1) * RotateZ(0);// tựa
	VeHinhLapPhuong(CarB_symbol, matrixPhanCapCarB);

}

void CarB_CuaXeTrai()
{
	//Cánh cửa
	CarB_vl = RGBtoColor(175, 100, 0);
	CarB_kt = RGBtoColor(160, 120, 20);
	CarB_pxg = RGBtoColor(150, 140, 40);
	TaoVatLieu(CarB_vl, CarB_kt, CarB_pxg, 1000);

	CarB_symbol = Translate(-0.925, 0, 0) * Scale(1.85, 1, 0.1);
	VeHinhLapPhuong(CarB_symbol, matrixPhanCapCarB);

	//Tay nắm cửa
	CarB_vl = RGBtoColor(120, 120, 100);
	CarB_kt = RGBtoColor(140, 122, 120);
	CarB_pxg = RGBtoColor(150, 140, 140);
	TaoVatLieu(CarB_vl, CarB_kt, CarB_pxg, 1000);

	CarB_symbol = Translate(-1.5, 0, -0.05) * Scale(0.1, 0.3, 0.3);
	VeHinhLapPhuong(CarB_symbol, matrixPhanCapCarB);
}

void CarB_CuaXePhai()
{
	//Cánh cửa
	CarB_vl = RGBtoColor(175, 100, 0);
	CarB_kt = RGBtoColor(160, 120, 20);
	CarB_pxg = RGBtoColor(150, 140, 40);
	TaoVatLieu(CarB_vl, CarB_kt, CarB_pxg, 1000);

	CarB_symbol = Translate(-0.925, 0, 0) * Scale(1.85, 1, 0.1);
	VeHinhLapPhuong(CarB_symbol, matrixPhanCapCarB);

	//Tay nắm cửa
	CarB_vl = RGBtoColor(120, 120, 100);
	CarB_kt = RGBtoColor(140, 122, 120);
	CarB_pxg = RGBtoColor(150, 140, 140);
	TaoVatLieu(CarB_vl, CarB_kt, CarB_pxg, 1000);

	CarB_symbol = Translate(-1.5, 0, 0.05) * Scale(0.1, 0.3, 0.3);
	VeHinhLapPhuong(CarB_symbol, matrixPhanCapCarB);
}


void CarB()
{
	matrixPhanCapCarB = mat4();
	matrixPhanCapCarB = matrixPhanCapCarB * Translate(CarB_DiChuyenX, 1, CarB_DiChuyenZ) * RotateY(CarB_XeXoay);;
	CarB_ThanXe();
	mat4 _MatrixTam = matrixPhanCapCarB;
	matrixPhanCapCarB = _MatrixTam * Translate(0, -1.5, 0) * RotateZ(CarB_BanhXeXoay);
	CarB_BanhSau();
	matrixPhanCapCarB = _MatrixTam * Translate(3.5, -1.5, -1.4) * RotateY(CarB_BanhTruocXoay) * RotateZ(CarB_BanhXeXoay);
	CarB_BanhTruoc();
	matrixPhanCapCarB = _MatrixTam * Translate(3.5, -1.5, 1.4) * RotateY(CarB_BanhTruocXoay) * RotateZ(CarB_BanhXeXoay);
	CarB_BanhTruoc();
	matrixPhanCapCarB = _MatrixTam * Translate(3.9, -0.4, -1.45) * RotateY(CarB_MoCuaXeTrai);
	CarB_CuaXeTrai();
	matrixPhanCapCarB = _MatrixTam * Translate(3.9, -0.4, 1.45) * RotateY(CarB_MoCuaXePhai);;
	CarB_CuaXePhai();
	matrixPhanCapCarB = _MatrixTam * Translate(1.95, -0.4, -1.45) * RotateY(CarB_MoCuaXeTrai);
	CarB_CuaXeTrai();
	matrixPhanCapCarB = _MatrixTam * Translate(1.95, -0.4, 1.45) * RotateY(CarB_MoCuaXePhai);;
	CarB_CuaXePhai();
}
#pragma endregion

#pragma region 
// Biến matrix dùng cho mô hình phân cấp
mat4 matrixPhanCapA;

color4 A_vl;
color4 A_kt;
color4 A_pxg;
mat4 A_symbol;

vec3 A_ViTriCu;
vec3 A_ViTriMoi;

void VachKe()
{
	float height = -1;
	float sizeChuan = 1;
	float sizeY = 0.1;
	float _leng = 100;
	A_vl = RGBtoColor(150, 150, 150);
	A_kt = RGBtoColor(150, 150, 150);
	A_pxg = RGBtoColor(150, 150, 150);
	TaoVatLieu(A_vl, A_kt, A_pxg, 100);
	A_symbol = Translate(10, height, 0) * Scale(sizeChuan, sizeY, _leng + 1);
	VeHinhLapPhuong(A_symbol, matrixPhanCapA);
	A_symbol = Translate(-10, height, 0) * Scale(sizeChuan, sizeY, _leng + 1);
	VeHinhLapPhuong(A_symbol, matrixPhanCapA);

	float _kc = 10;
	for (int i = 0; i < _leng / _kc + 1; i++)
	{
		A_symbol = Translate(0, height, (i * _kc) - 50) * Scale(20, sizeY, sizeChuan);
		VeHinhLapPhuong(A_symbol, matrixPhanCapA);
	}
}

void PhanCachDuongVoiBaiXe()
{
	A_vl = RGBtoColor(130, 130, 65);
	A_kt = RGBtoColor(130, 130, 65);
	A_pxg = RGBtoColor(130, 130, 65);
	TaoVatLieu(A_vl, A_kt, A_pxg, 100);
	A_symbol = Translate(-90, -1, 0) * Scale(20, 0.1, 200);
	VeHinhLapPhuong(A_symbol, matrixPhanCapA);
}

void FuncA()
{
	matrixPhanCapA = mat4();
	mat4 mat1 = matrixPhanCapA;
	PhanCachDuongVoiBaiXe();
	matrixPhanCapA = mat1 * Translate(60, 0, 0);
	VachKe();
	matrixPhanCapA = mat1 * Translate(0, 0, -70) * RotateY(-100);
	VachKe();
	matrixPhanCapA = mat1 * Translate(0, 0, 70) * RotateY(90);
	VachKe();
}
#pragma endregion A

#pragma region
// Biến matrix dùng cho mô hình phân cấp
mat4 matrixPhanCapCarA;
color4 CarA_vl;
color4 CarA_kt;
color4 CarA_pxg;
mat4 CarA_symbol;

// Biến dùng để di chuyển
float CarA_DiChuyenX = 0;
float CarA_DiChuyenZ = 0;
float CarA_XeXoay = 0;
float CarA_BanhTruocXoay = 0;
float CarA_MoThungXe = 0;
float CarA_BanhXeXoay = 0;
float CarA_MoCuaXeTrai = 0;
float CarA_MoCuaXePhai = 0;
float CarA_DenXe[] = { 220,60,20 };

void CarA_BanhSau()
{
	CarA_vl = RGBtoColor(30, 20, 30);
	CarA_kt = RGBtoColor(35, 30, 40);
	CarA_pxg = RGBtoColor(40, 40, 50);
	TaoVatLieu(CarA_vl, CarA_kt, CarA_pxg, 1000);

	// Bánh trái sau
	CarA_symbol = Translate(0, 0, -1.4) * Scale(1, 1, 0.3) * RotateX(90);
	VeHinhTru(CarA_symbol, matrixPhanCapCarA);

	// Bánh phải sau
	CarA_symbol = Translate(0, 0, 1.4) * Scale(1, 1, 0.3) * RotateX(90);
	VeHinhTru(CarA_symbol, matrixPhanCapCarA);

	//Trục bánh xe
	CarA_vl = RGBtoColor(82, 135, 190);
	CarA_kt = RGBtoColor(75, 120, 180);
	CarA_pxg = RGBtoColor(70, 110, 170);
	TaoVatLieu(CarA_vl, CarA_kt, CarA_pxg, 1000);
	CarA_symbol = Scale(0.3, 0.3, 3.2) * RotateX(90);
	VeHinhTru(CarA_symbol, matrixPhanCapCarA);
}

void CarA_BanhTruoc()
{
	CarA_vl = RGBtoColor(30, 20, 30);
	CarA_kt = RGBtoColor(35, 30, 40);
	CarA_pxg = RGBtoColor(40, 40, 50);
	TaoVatLieu(CarA_vl, CarA_kt, CarA_pxg, 1000);

	CarA_symbol = Scale(1, 1, 0.3) * RotateX(90);
	VeHinhTru(CarA_symbol, matrixPhanCapCarA);

	//Trục bánh xe
	CarA_vl = RGBtoColor(82, 135, 190);
	CarA_kt = RGBtoColor(75, 120, 180);
	CarA_pxg = RGBtoColor(70, 110, 170);
	TaoVatLieu(CarA_vl, CarA_kt, CarA_pxg, 1000);
	CarA_symbol = Scale(0.3, 0.3, 0.36) * RotateX(90);
	VeHinhTru(CarA_symbol, matrixPhanCapCarA);
}

void CarA_ThanXe()
{
	CarA_vl = RGBtoColor(10, 100, 220);
	CarA_kt = RGBtoColor(30, 120, 200);
	CarA_pxg = RGBtoColor(50, 130, 180);
	TaoVatLieu(CarA_vl, CarA_kt, CarA_pxg, 1000);

	//Mặt dưới đầu xe
	CarA_symbol = Translate(3.05, -0.95, 0) * Scale(2.1, 0.1, 3);
	VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);

	//Mặt sau đầu xe
	CarA_symbol = Translate(2, 0, 0) * Scale(0.1, 2, 3);
	VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);

	//Mặt trước đầu xe
	CarA_symbol = Translate(3.95, -0.4, 0) * Scale(0.1, 1.1, 3);
	VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);

	//Mặt trên đầu xe
	CarA_symbol = Translate(2.55, 0.95, 0) * Scale(1.2, 0.1, 3);
	VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);

	//Thanh chéo trước đầu xe bên trái
	CarA_symbol = Translate(3.53, 0.54, -1.4) * RotateZ(-45) * Scale(1.2, 0.1, 0.2);
	VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);

	//Thanh chéo trước đầu xe bên trái
	CarA_symbol = Translate(3.53, 0.54, 1.4) * RotateZ(-45) * Scale(1.2, 0.1, 0.2);
	VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);

	CarA_vl = RGBtoColor(200, 80, 80);
	CarA_kt = RGBtoColor(190, 90, 100);
	CarA_pxg = RGBtoColor(180, 100, 120);
	TaoVatLieu(CarA_vl, CarA_kt, CarA_pxg, 1000);

	// Thanh dưới thùng xe bên trái
	CarA_symbol = Translate(-1, -0.95, -1.25) * Scale(6, 0.1, 0.5);
	VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);

	//Thanh dưới thùng xe ở giữa
	CarA_symbol = Translate(-1, -0.95, 0) * Scale(6, 0.1, 0.5);
	VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);

	// Thanh dưới thùng xe bên phải
	CarA_symbol = Translate(-1, -0.95, 1.25) * Scale(6, 0.1, 0.5);
	VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);

	//Thanh dưới thùng xe phía sau
	CarA_symbol = Translate(-3.75, -0.95, 0) * Scale(0.5, 0.1, 3);
	VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);

	//Thanh dưới thùng xe phía trước
	CarA_symbol = Translate(1.5, -0.95, 0) * Scale(1, 0.1, 3);
	VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);

	//Trang trí đầu xe
	CarA_vl = RGBtoColor(175, 100, 60);
	CarA_kt = RGBtoColor(160, 120, 80);
	CarA_pxg = RGBtoColor(150, 140, 100);
	TaoVatLieu(CarA_vl, CarA_kt, CarA_pxg, 1000);
	// Nóc xe
	for (size_t i = 0; i < 3; i++)
	{
		CarA_symbol = Translate((float)i / 3 + 2.2, 0.95, 0) * Scale(0.1, 0.14, 2.8);
		VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);
	}

	// Hình trụ đầu xe
	CarA_symbol = Translate(4.03, -0.93, 0) * RotateX(90) * Scale(0.2, 3.02, 0.2);
	VeHinhTru(CarA_symbol, matrixPhanCapCarA);

	// Đèn xe
	CarA_vl = RGBtoColor(CarA_DenXe[0], CarA_DenXe[1], CarA_DenXe[2]);
	CarA_kt = RGBtoColor(CarA_DenXe[0] + 20, CarA_DenXe[1] + 20, CarA_DenXe[2] + 20);
	CarA_pxg = RGBtoColor(CarA_DenXe[0] + 40, CarA_DenXe[1] + 40, CarA_DenXe[2] + 40);
	TaoVatLieu(CarA_vl, CarA_kt, CarA_pxg, 1000);

	CarA_symbol = Translate(4, -0.5, 1) * RotateZ(90) * Scale(0.4, 0.1, 0.4);
	VeHinhTru(CarA_symbol, matrixPhanCapCarA);

	CarA_symbol = Translate(4, -0.5, -1) * RotateZ(90) * Scale(0.4, 0.1, 0.4);
	VeHinhTru(CarA_symbol, matrixPhanCapCarA);

	//Ghế
	CarA_vl = RGBtoColor(129, 156, 96);
	CarA_kt = RGBtoColor(129, 156, 96);
	CarA_pxg = RGBtoColor(129, 156, 96);
	TaoVatLieu(CarA_vl, CarA_kt, CarA_pxg, 1000);
	CarA_symbol = Translate(2.6, -0.8, 0) * Scale(0.8, 0.3, 1.2);// ghế
	VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);
	CarA_symbol = Translate(2.25, -0.15, 0) * Scale(0.1, 1.2, 1.2) * RotateZ(0);// tựa
	VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);
}

void CarA_VoLang()
{

	CarA_vl = RGBtoColor(175, 100, 60);
	CarA_kt = RGBtoColor(160, 120, 80);
	CarA_pxg = RGBtoColor(150, 140, 100);
	TaoVatLieu(CarA_vl, CarA_kt, CarA_pxg, 1000);

	// Thanh Dài
	CarA_symbol = Translate(0, 0.5, 0) * Scale(0.1, 1, 0.1);
	VeHinhTru(CarA_symbol, matrixPhanCapCarA);

	//Bánh Xoay Vô Lăng
	CarA_symbol = Translate(0, 1, 0) * Scale(0.5, 0.1, 0.5);
	VeHinhTru(CarA_symbol, matrixPhanCapCarA);
}

void CarA_CuaXeTrai()
{
	//Cánh cửa
	CarA_vl = RGBtoColor(175, 100, 0);
	CarA_kt = RGBtoColor(160, 120, 20);
	CarA_pxg = RGBtoColor(150, 140, 40);
	TaoVatLieu(CarA_vl, CarA_kt, CarA_pxg, 1000);

	CarA_symbol = Translate(-0.925, 0, 0) * Scale(1.85, 1, 0.1);
	VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);

	//Tay nắm cửa
	CarA_vl = RGBtoColor(120, 120, 100);
	CarA_kt = RGBtoColor(140, 122, 120);
	CarA_pxg = RGBtoColor(150, 140, 140);
	TaoVatLieu(CarA_vl, CarA_kt, CarA_pxg, 1000);

	CarA_symbol = Translate(-1.5, 0, -0.05) * Scale(0.1, 0.3, 0.3);
	VeHinhTru(CarA_symbol, matrixPhanCapCarA);
}

void CarA_CuaXePhai()
{
	//Cánh cửa
	CarA_vl = RGBtoColor(175, 100, 0);
	CarA_kt = RGBtoColor(160, 120, 20);
	CarA_pxg = RGBtoColor(150, 140, 40);
	TaoVatLieu(CarA_vl, CarA_kt, CarA_pxg, 1000);

	CarA_symbol = Translate(-0.925, 0, 0) * Scale(1.85, 1, 0.1);
	VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);

	//Tay nắm cửa
	CarA_vl = RGBtoColor(120, 120, 100);
	CarA_kt = RGBtoColor(140, 122, 120);
	CarA_pxg = RGBtoColor(150, 140, 140);
	TaoVatLieu(CarA_vl, CarA_kt, CarA_pxg, 1000);

	CarA_symbol = Translate(-1.5, 0, 0.05) * Scale(0.1, 0.3, 0.3);
	VeHinhTru(CarA_symbol, matrixPhanCapCarA);
}

void CarB_ThungXe()
{
	CarA_vl = RGBtoColor(150, 130, 30);
	CarA_kt = RGBtoColor(140, 120, 20);
	CarA_pxg = RGBtoColor(130, 110, 10);
	TaoVatLieu(CarA_vl, CarA_kt, CarA_pxg, 1000);

	//Mặt dưới
	CarA_symbol = Translate(2.75, 0.1, 0) * Scale(5.6, 0.2, 3.6);
	VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);

	//Mặt trái
	CarA_symbol = Translate(2.75, 1, 1.65) * Scale(5.5, 2, 0.1);
	VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);

	// Mặt phải
	CarA_symbol = Translate(2.75, 1, -1.65) * Scale(5.5, 2, 0.1);
	VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);

	//Mặt trước
	CarA_symbol = Translate(5.5, 1, 0) * Scale(0.1, 2, 3.6);
	VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);

	CarA_symbol = Translate(2.75, 1.9, 1.7) * Scale(5.5, 0.2, 0.2);
	VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);

	CarA_symbol = Translate(0, 1, 1.7) * Scale(0.1, 2, 0.2);
	VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);

	CarA_symbol = Translate(2.75, 1.9, -1.7) * Scale(5.5, 0.2, 0.2);
	VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);

	CarA_symbol = Translate(0, 1, -1.7) * Scale(0.1, 2, 0.2);
	VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);

	for (int i = 1; i <= 11; i++)
	{
		CarA_symbol = Translate((float)i / 2, 1, 1.7) * Scale(0.1, 2, 0.1);
		VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);
	}

	for (int i = 1; i <= 11; i++)
	{
		CarA_symbol = Translate((float)i / 2, 1, -1.7) * Scale(0.1, 2, 0.1);
		VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);
	}

	CarA_vl = RGBtoColor(192, 98, 58);
	CarA_kt = RGBtoColor(192, 98, 58);
	CarA_pxg = RGBtoColor(192, 98, 58);
	TaoVatLieu(CarA_vl, CarA_kt, CarA_pxg, 1000);

}

void CarA_CuaThungXe()
{
	CarA_vl = RGBtoColor(150, 110, 10);
	CarA_kt = RGBtoColor(130, 110, 20);
	CarA_pxg = RGBtoColor(130, 110, 30);
	TaoVatLieu(CarA_vl, CarA_kt, CarA_pxg, 1000);

	CarA_symbol = Translate(0, -0.9, 0) * Scale(0.1, 1.8, 3.2);
	VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);

	CarA_vl = RGBtoColor(115, 10, 10);
	CarA_kt = RGBtoColor(130, 30, 20);
	CarA_pxg = RGBtoColor(140, 40, 30);
	TaoVatLieu(CarA_vl, CarA_kt, CarA_pxg, 1000);

	CarA_symbol = Translate(0, -0.05, 0) * RotateX(90) * Scale(0.05, 3.65, 0.05);
	VeHinhTru(CarA_symbol, matrixPhanCapCarA);

	for (size_t i = 0; i < 4; i++)
	{
		CarA_symbol = Translate(-0.05, (float)i / 3 - 1.5, 0) * Scale(0.05, 0.05, 3);
		VeHinhLapPhuong(CarA_symbol, matrixPhanCapCarA);
	}

}

void CarA_BenNangThung()
{
	CarA_vl = RGBtoColor(195, 90, 50);
	CarA_kt = RGBtoColor(200, 100, 65);
	CarA_pxg = RGBtoColor(205, 110, 70);
	TaoVatLieu(CarA_vl, CarA_kt, CarA_pxg, 1000);

	CarA_symbol = Translate(0, 0.5, 0) * Scale(0.3, 1, 0.3);
	VeHinhTru(CarA_symbol, matrixPhanCapCarA);
}

void CarA()
{
	matrixPhanCapCarA = mat4();
	matrixPhanCapCarA = matrixPhanCapA * Translate(30, 0, 20);
	matrixPhanCapCarA = matrixPhanCapCarA * Translate(CarA_DiChuyenX, 1, CarA_DiChuyenZ) * RotateY(CarA_XeXoay);
	CarA_ThanXe();
	mat4 _MatrixTam = matrixPhanCapCarA;
	matrixPhanCapCarA = matrixPhanCapCarA * Translate(-4, -0.85, 0) * RotateZ(CarA_MoThungXe);
	CarB_ThungXe();
	matrixPhanCapCarA = matrixPhanCapCarA * Translate(0, 2, 0) * *RotateZ(-CarA_MoThungXe);
	CarA_CuaThungXe();
	matrixPhanCapCarA = _MatrixTam * Translate(-2, -1.5, 0) * RotateZ(CarA_BanhXeXoay);
	CarA_BanhSau();
	matrixPhanCapCarA = _MatrixTam * Translate(3.9, -0.4, -1.45) * RotateY(CarA_MoCuaXeTrai);
	CarA_CuaXeTrai();
	matrixPhanCapCarA = _MatrixTam * Translate(3.9, -0.4, 1.45) * RotateY(CarA_MoCuaXePhai);;
	CarA_CuaXePhai();
	matrixPhanCapCarA = _MatrixTam * Translate(2.5, -1.5, -1.4) * RotateY(CarA_BanhTruocXoay) * RotateZ(CarA_BanhXeXoay);
	CarA_BanhTruoc();
	matrixPhanCapCarA = _MatrixTam * Translate(2.5, -1.5, 1.4) * RotateY(CarA_BanhTruocXoay) * RotateZ(CarA_BanhXeXoay);
	CarA_BanhTruoc();
	matrixPhanCapCarA = _MatrixTam * Translate(-0.3, -1, 0) * RotateZ(30) * Scale(1, 4.1 * sinf(DEGtoRAD(CarA_MoThungXe)), 1);
	CarA_BenNangThung();
	matrixPhanCapCarA = _MatrixTam * Translate(3.9, -0.7, 0) * RotateZ(30) * RotateY(CarA_BanhTruocXoay * 2);
	CarA_VoLang();

}
#pragma endregion CarA

#pragma region  
mat4 matrixPhanCapMoiTruong;

color4 MT_vl;
color4 MT_kt;
color4 MT_pxg;

float vtXe[15];
float _vtXe[15];
color4 colorNha[4] = { RGBtoColor(63,72,204),RGBtoColor(63 ,204,70), RGBtoColor(204,176,63), RGBtoColor(148,63,204) };

void XeCo(color4 _color, float _vt)
{
	A_vl = _color;
	A_kt = _color;
	A_pxg = _color;
	TaoVatLieu(A_vl, A_kt, A_pxg, 1000);
	A_symbol = Translate(-105, -1, _vt) * Scale(3, 2, 6) * Translate(0, 0.5, 0);
	VeHinhLapPhuong(A_symbol, matrixPhanCapA);
}
void _XeCo(color4 _color, float _vt)
{
	A_vl = _color;
	A_kt = _color;
	A_pxg = _color;
	TaoVatLieu(A_vl, A_kt, A_pxg, 1000);
	A_symbol = Translate(-105, -1, _vt) * Scale(3, 2, 6) * Translate(0, 0.5, 0);
	VeHinhLapPhuong(A_symbol, matrixPhanCapA);
}

void MT_Duong()
{
	A_vl = RGBtoColor(10, 10, 10);
	A_kt = RGBtoColor(30, 30, 30);
	A_pxg = RGBtoColor(30, 30, 30);
	TaoVatLieu(A_vl, A_kt, A_pxg, 100);
	A_symbol = Translate(-110, -1, 0) * Scale(20, 0.01, 1000);
	VeHinhLapPhuong(A_symbol, matrixPhanCapA);

	A_vl = RGBtoColor(150, 150, 150);
	A_kt = RGBtoColor(150, 150, 150);
	A_pxg = RGBtoColor(150, 150, 150);
	TaoVatLieu(A_vl, A_kt, A_pxg, 100);
	A_symbol = Translate(-110, -0.99, 0) * Scale(1, 0.01, 1000);
	VeHinhLapPhuong(A_symbol, matrixPhanCapA);

	for (int i = 0; i < 15; i++)
	{
		XeCo(colorNha[i % 4], vtXe[i]);
	}
	for (int i = 0; i < 15; i++)
	{
		_XeCo(colorNha[i % 4], _vtXe[i]);
	}
}

void Nha(color4 _color, mat4 _trans, float _cao)
{
	A_vl = _color;
	A_kt = _color;
	A_pxg = _color;
	TaoVatLieu(A_vl, A_kt, A_pxg, 100);
	A_symbol = _trans * Translate(0, -1, 0) * Scale(10, _cao, 10) * Translate(0, 0.5, 0);
	VeHinhLapPhuong(A_symbol, matrixPhanCapA);
}


// Vẽ cái sân
void Cai_San()
{
	for (int i = 0; i < 20; i++)
	{
		Nha(colorNha[i % 4], Translate(95, 0, -95 + i * 10), (i % 4 + 1) * 10);
	}
	for (int i = 0; i <= 20; i++)
	{
		Nha(colorNha[i % 4], Translate(-105 + i * 10, 0, -105), (i % 4 + 1) * 10);
	}
	for (int i = 0; i <= 20; i++)
	{
		Nha(colorNha[i % 4], Translate(-95 + i * 10, 0, 105), (i % 4 + 1) * 10);
	}


	MT_Duong();
	// Đất
	A_vl = RGBtoColor(10, 10, 10);
	A_kt = RGBtoColor(15, 15, 15);
	A_pxg = RGBtoColor(15, 15, 15);
	TaoVatLieu(A_vl, A_kt, A_pxg, 1000);
	A_symbol = Translate(0, -1, 0) * Scale(200, 0.01, 200);
	VeHinhLapPhuong(A_symbol, matrixPhanCapA);

}

void MoiTruong()
{
	matrixPhanCapMoiTruong = mat4();
	Cai_San();
}
#pragma endregion Môi Trường

#pragma region 
void DisplayModels(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	matrixPhanCapA = mat4();
	MoiTruong();
	FuncA();
	CarA();
	CarB();
	vec3 cam_forward = normalize(vec3(cosf(DEGtoRAD(cam_Rotation[0])) * abs(sinf(DEGtoRAD(cam_Rotation[1]))),
		cosf(DEGtoRAD(cam_Rotation[1])),
		cosf(DEGtoRAD(cam_Rotation[2])) * abs(sinf(DEGtoRAD(cam_Rotation[1])))));

	at = eye + cam_forward;
	view = LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, view);
	projection = Frustum(-0.01, 0.01, -0.01, 0.01, 0.02, 50);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection);
	glViewport(0, 0, 1000, 1000);
	glutPostRedisplay();
	glutSwapBuffers();
}

bool Key_CarA = false;
bool Key_CarB = false;

void KeyboardControl(unsigned char key, int x, int y)
{
	vec3 cam_forward = normalize(vec3(cosf(DEGtoRAD(cam_Rotation[0])) * abs(sinf(DEGtoRAD(cam_Rotation[1]))),
		cosf(DEGtoRAD(cam_Rotation[1])),
		cosf(DEGtoRAD(cam_Rotation[2])) * abs(sinf(DEGtoRAD(cam_Rotation[1])))));
	vec3 cam_right = normalize(cross(cam_forward, up));
	vec4 cam_up = normalize(cross(cam_right, cam_forward));

	switch (key) {
		/*case 033:
			exit(1);
			break;*/
	case 'f':
		cam_Rotation[0] += 10 * (che_do_chi_tiet ? 0.1 : 1);
		cam_Rotation[2] += 10 * (che_do_chi_tiet ? 0.1 : 1);
		break;
	case 'h':
		cam_Rotation[0] -= 10 * (che_do_chi_tiet ? 0.1 : 1);
		cam_Rotation[2] -= 10 * (che_do_chi_tiet ? 0.1 : 1);
		break;
	case 't':
		cam_Rotation[1] -= 10 * (che_do_chi_tiet ? 0.1 : 1);
		if (cam_Rotation[1] <= 10) cam_Rotation[1] = 10;
		break;
	case 'g':
		cam_Rotation[1] += 10 * (che_do_chi_tiet ? 0.1 : 1);
		if (cam_Rotation[1] >= 170) cam_Rotation[1] = 170;
		break;
	case 'w':
		eye = eye + cam_forward * (che_do_chi_tiet ? 0.1 : 1);
		break;
	case 's':
		eye = eye - cam_forward * (che_do_chi_tiet ? 0.1 : 1);
		break;
	case 'a':
		eye = eye - cam_right * (che_do_chi_tiet ? 0.1 : 1);
		break;
	case 'd':
		eye = eye + cam_right * (che_do_chi_tiet ? 0.1 : 1);
		break;
	case 'q':
		eye = eye - cam_up * (che_do_chi_tiet ? 0.1 : 1);
		break;
	case 'e':
		eye = eye + cam_up * (che_do_chi_tiet ? 0.1 : 1);
		break;
	case 'r':
		che_do_chi_tiet = !che_do_chi_tiet;
		break;
	case 'z':
		Key_CarA = true;
		Key_CarB = false;
		break;
	case 'x':
		Key_CarA = false;
		Key_CarB = true;
		break;
	}

	if (Key_CarA)
	{
		switch (key)
		{
		case 'i':
			CarA_XeXoay += CarA_BanhTruocXoay * 0.3 / 7.5;
			CarA_BanhXeXoay += (0.3 / (0.5 * 2 * M_PI)) * 360;
			CarA_DiChuyenX += cosf(DEGtoRAD(CarA_XeXoay)) * 0.3;
			CarA_DiChuyenZ += -sinf(DEGtoRAD(CarA_XeXoay)) * 0.3;
			break;
		case 'k':
			CarA_XeXoay -= CarA_BanhTruocXoay * 0.3 / 7.5;
			CarA_BanhXeXoay -= (0.3 / (0.5 * 2 * M_PI)) * 360;
			CarA_DiChuyenX -= cosf(DEGtoRAD(CarA_XeXoay)) * 0.3;
			CarA_DiChuyenZ -= -sinf(DEGtoRAD(CarA_XeXoay)) * 0.3;
			break;
		case 'j':
			CarA_BanhTruocXoay += 5;
			if (CarA_BanhTruocXoay >= 30)
			{
				CarA_BanhTruocXoay = 30;
			}
			break;
		case 'l':
			CarA_BanhTruocXoay -= 5;
			if (CarA_BanhTruocXoay <= -30)
			{
				CarA_BanhTruocXoay = -30;
			}
			break;
		case 'm':
			CarA_MoThungXe += 5;
			if (CarA_MoThungXe >= 50)
			{
				CarA_MoThungXe = 50;
			}
			break;
		case 'M':
			CarA_MoThungXe -= 5;
			if (CarA_MoThungXe <= 0)
			{
				CarA_MoThungXe = 0;
			}
			break;
		case 'o':
			CarA_MoCuaXeTrai -= 5;
			if (CarA_MoCuaXeTrai <= -50)
			{
				CarA_MoCuaXeTrai = -50;
			}
			break;
		case 'O':
			CarA_MoCuaXeTrai += 5;
			if (CarA_MoCuaXeTrai >= 0)
			{
				CarA_MoCuaXeTrai = 0;
			}
			break;
		case 'p':
			CarA_MoCuaXePhai += 5;
			if (CarA_MoCuaXePhai >= 50)
			{
				CarA_MoCuaXePhai = 50;
			}
			break;
		case 'P':
			CarA_MoCuaXePhai -= 5;
			if (CarA_MoCuaXePhai <= 0)
			{
				CarA_MoCuaXePhai = 0;
			}
			break;
		case '.':
			CarA_DenXe[0] = 220;
			CarA_DenXe[1] = 60;
			CarA_DenXe[2] = 20;
			break;
		case '/':
			CarA_DenXe[0] = 80;
			CarA_DenXe[1] = 210;
			CarA_DenXe[2] = 60;
			break;
		}
	}

	if (Key_CarB)
	{
		switch (key)
		{
		case 'i':
			CarB_XeXoay += CarB_BanhTruocXoay * 0.3 / 7.5;
			CarB_BanhXeXoay += (0.3 / (0.5 * 2 * M_PI)) * 360;
			CarB_DiChuyenX += cosf(DEGtoRAD(CarB_XeXoay)) * 0.3;
			CarB_DiChuyenZ += -sinf(DEGtoRAD(CarB_XeXoay)) * 0.3;
			break;
		case 'k':
			CarB_XeXoay -= CarB_BanhTruocXoay * 0.3 / 7.5;
			CarB_BanhXeXoay -= (0.3 / (0.5 * 2 * M_PI)) * 360;
			CarB_DiChuyenX -= cosf(DEGtoRAD(CarB_XeXoay)) * 0.3;
			CarB_DiChuyenZ -= -sinf(DEGtoRAD(CarB_XeXoay)) * 0.3;
			break;
		case 'j':
			CarB_BanhTruocXoay += 5;
			if (CarB_BanhTruocXoay >= 30)
			{
				CarB_BanhTruocXoay = 30;
			}
			break;
		case 'l':
			CarB_BanhTruocXoay -= 5;
			if (CarB_BanhTruocXoay <= -30)
			{
				CarB_BanhTruocXoay = -30;
			}
			break;
		case 'o':
			CarB_MoCuaXeTrai -= 5;
			if (CarB_MoCuaXeTrai <= -50)
			{
				CarB_MoCuaXeTrai = -50;
			}
			break;
		case 'O':
			CarB_MoCuaXeTrai += 5;
			if (CarB_MoCuaXeTrai >= 0)
			{
				CarB_MoCuaXeTrai = 0;
			}
			break;
		case 'p':
			CarB_MoCuaXePhai += 5;
			if (CarB_MoCuaXePhai >= 50)
			{
				CarB_MoCuaXePhai = 50;
			}
			break;
		case 'P':
			CarB_MoCuaXePhai -= 5;
			if (CarB_MoCuaXePhai <= 0)
			{
				CarB_MoCuaXePhai = 0;
			}
			break;
		}
	}
}

void IdleControl()
{

	for (int i = 0; i < 15; i++)
	{
		vtXe[i] += 0.1;
		if (500 < vtXe[i]) vtXe[i] = -500;
		_vtXe[i] -= 0.1;
		if (-500 > _vtXe[i]) _vtXe[i] = 500;
	}
}
#pragma endregion Các hàm hiển thị model, camera, bàn phím.

int main(int argc, char** argv)
{
	for (int i = 0; i < 15; i++)
	{
		vtXe[i] = -i * 70;
		_vtXe[i] = i * 70;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("DHMT");


	glewInit();

	GenerateCacHinh();
	KhoiTaoBoDemGPU();
	SetupShader();

	glutDisplayFunc(DisplayModels);
	glutKeyboardFunc(KeyboardControl);
	glutIdleFunc(IdleControl);
	//glutReshapeFunc(CameraControl);

	glutMainLoop();
	return 0;
}