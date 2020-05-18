
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <dirent.h>
#include <sys/types.h>



#include <vector>

// Librerías de OpenCV 4
#include <opencv2/opencv.hpp>

#include <opencv2/highgui/highgui_c.h>
#include <opencv2/cvconfig.h>


using namespace std;
using namespace cv;



Mat imagen;

Mat  histogramaGris(Mat copia) {
	int maximo = 256;
	float rango[] = { 0.f, 255.f };
	const float* pRango = { rango };
	Mat hist, gris;
	cvtColor(copia, gris, CV_BGR2GRAY);
	vector<Mat> canales;
	split(imagen, canales);

	calcHist(&canales[0], 1, 0, Mat(), hist, 1, &maximo, &pRango, true, false);
	int limite = (sizeof(hist));
	//cout << limite << endl;
	return hist;
}

Mat cargarImagen(const char* path) {
	//string ruta = "C:/Users/javie/source/repos/Project1/Project1/SanBasilio.jpg";
	imagen = imread(path);
	Mat hist=histogramaGris(imagen);
	return hist;
}
int tamanioDirectorio(const char* path) {
	int contador = 0;
	struct dirent* entry;
	DIR* dir = opendir(path);
	if (dir == NULL) {
		return 0;
	}
	while ((entry = readdir(dir)) != NULL) {
		contador +=  1;
	}

	closedir(dir);
	return contador-2;
}
void leerCarpetas(const char* path) {
	//ruta del archivo
	string nombreTrain = "/train.csv";
	string nombreTest = "/test.csv";

	string rutaTrain(path + nombreTrain);
	string rutaTest(path + nombreTest);

	const char* pathTrain = rutaTrain.c_str();
	const char* pathTest = rutaTest.c_str();
	cout << "Ruta" << rutaTrain << endl;

	// guardar en archivo
	ofstream file, file2;
	file.open(pathTrain);
	file2.open(pathTest);
	Mat hist;

	int tamanio= tamanioDirectorio(path);

	int tamanioTrain=round(tamanio*90/100);
	int tamanioTest = round(tamanio * 10 / 100);
	int contador = 1;
	struct dirent* entry;
	DIR* dir = opendir(path);
	if (dir == NULL) {
		return;
	}
	while ((entry = readdir(dir)) != NULL) {
		string nombre = entry->d_name;
		
		if (nombre._Equal(".") or nombre._Equal("..") or nombre._Equal("test.txt") or nombre._Equal("train.txt") or nombre._Equal("test.csv") or nombre._Equal("train.csv")) {
			//contador --;
		}
		else {
			nombre = "/" + nombre;
			string str(path + nombre);
			const char* rutaImagen = str.c_str();
			hist = cargarImagen(rutaImagen);
			if (contador <= tamanioTrain) {
				//file << hist << endl;
				for (int i = 0; i < 256; i++)
				{
					file << hist.at<float>(i, 0) << ";";
				}
				file << "\n";
				
			}
			else {
				//file2 << hist << endl; 
				for (int i = 0; i < 256; i++)
				{
					file2 << hist.at<float>(i, 0) << ";";
				}
				file2 << "\n";
			}
			contador++;
		}
		
	}
	file.close();
	file2.close();
	closedir(dir);
}
void list_dir(const char* path) {
	struct dirent* entry;
	DIR* dir = opendir(path);
	if (dir == NULL) {
		return;
	}
	while ((entry = readdir(dir)) != NULL) {
		string nombre = entry->d_name;
		if (nombre._Equal(".") or nombre._Equal("..")) {

		}
		else {
			nombre = "/" + nombre;
			string str(path + nombre);
			const char* rutaCarpeta = str.c_str();
			cout << rutaCarpeta << endl;
			leerCarpetas(rutaCarpeta);
		}
		
	}
	closedir(dir);
}


int main(int argc, char* argv[]) {

	const char*ruta = "C:/Users/javie/Desktop/101_ObjectCategories/101_ObjectCategories";
	list_dir(ruta);
	//leerCarpetas("C:/Users/javie/Desktop/101_ObjectCategories/101_ObjectCategories/bonsai");
}