#include<opencv2/opencv.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include<iostream>
#include<vector>
#include<stdint.h>
#include<conio.h>
#include<math.h>
#include<list>
#include<algorithm>
#include<fstream>
#include<string>

using namespace std;
//using namespace cv;

vector<vector<int>> coord;
int size_x = 0, size_y = 0;
bool verify = false;
int neighborhood = 20;
cv::Mat img;

//Scalar colorTab[] =
//	{
//		Scalar(0, 0, 255),
//		Scalar(0,255,0),
//		Scalar(255,100,100),
//		Scalar(255,0,255),
//		Scalar(0,255,255)
//	};

struct weight
{
	struct data
	{
		vector<int> c;
		int pos;
		string color = "white";
		data* ancestor = NULL;
		int cluster=-1;
	};
	data* link = new data;
	double w;
};

vector<weight::data*> v;
vector<vector<vector<weight*>>> graphs;

void convert(string s)
{
	int num1, num2;
	string tmp;
	bool check = false;
	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] == ';')
		{
			check = true;
			num1 = stoi(tmp);
			tmp = "";
			continue;
		}
		if (!check)
			tmp += s[i];
		else
			tmp += s[i];
	}
	num2 = stoi(tmp);
	coord.resize(coord.size() + 1);
	coord[coord.size() - 1].push_back(num1);
	coord[coord.size() - 1].push_back(num2);
	if (num1 > size_x)
		size_x = num1;
	if (num2 > size_y)
		size_y = num2;
}

double euclidian(double x1, double y1, double x0, double y0)
{
	return sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0));
}

void opencv()
{
	cv::Scalar colorTab[] =
	{
		cv::Scalar(0, 0, 255),
		cv::Scalar(0,255,0),
		cv::Scalar(255,100,100),
		cv::Scalar(255,0,255),
		cv::Scalar(0,255,255)
	};
	cv::Mat img(size_x, size_y, CV_8UC3);

	cv::Mat points(coord.size(), 1, CV_32FC2), labels;
	cv::Mat centers;
	img = cv::Scalar::all(0);
	
	for (int i = 0; i < v.size(); i++)
	{
		if (graphs[v[i]->cluster].size() > 100)
		{
			cv::Point p;
			p.x = v[i]->c[0];
			p.y = v[i]->c[1];
			circle(img, p, 2, colorTab[v[i]->cluster], cv::FILLED, cv::LINE_AA);
		}
	}
	//cv::imshow("clusters", img);
	vector<int> compression_params;
	compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);
	imwrite("mask6_result.jpg", img, compression_params);
	//cv::waitKey();
}
void dfs(weight::data* u)
{
	u->color = "gray";
	if (u->pos == 2042)
		int kek = 2;
	//cout << u->pos << endl;
	for (int i = 0; i < v.size(); i++)
	{
		double dist = euclidian(v[i]->c[0], v[i]->c[1], u->c[0], u->c[1]);
		if (v[i]->color=="white" && dist < neighborhood && v[i]!=u)
		{
			weight* r = new weight;
			weight* w = new weight;
			if (graphs[graphs.size() - 1].size() == 0)
			{
				v[i]->ancestor = u;
				r->link = v[i];
				w->link = u;
				r->w = dist;
				w->w = dist;
				r->link->cluster = w->link->cluster = graphs.size()-1;
				/*graph[0].push_back(w);
				graph[1].push_back(r);*/
				w->link->pos = 0;
				r->link->pos = 1;
				graphs[graphs.size() - 1].resize(2);
				graphs[graphs.size() - 1][0].push_back(r);
				graphs[graphs.size() - 1][1].push_back(w);
				v[i]->color = "gray";
				dfs(v[i]);
			}
			else
			{
				v[i]->ancestor = u;
				r->link = v[i];
				w->link = u;
				r->link->cluster = w->link->cluster;
				r->w = w->w = dist;
				graphs[u->cluster][u->pos].push_back(r);
				graphs[u->cluster].resize(graphs[u->cluster].size() + 1);
				v[i]->pos = graphs[u->cluster].size() - 1;
				graphs[u->cluster][v[i]->pos].push_back(w);
				//v[i]->color = "gray";
				//cout << v[i]->pos << endl;
				dfs(v[i]);
			}
		}
	}
	u->color = "black";
}

//vector of graphs need to be created;
int main()
{
	ifstream  fin("mask_5.csv");
	string value;
	//cout << "Enter neighborhood: ";
	//cin >> neighborhood;
	int iterations = 0;
	while (getline(fin, value))
	{
		convert(value);
		weight::data* u = new weight::data;
		int size = coord.size();
		v.resize(size);
		//graph.resize(size);
		u->c = coord[size-1];
		v[size - 1] = u;
		//if (iterations == 8)
		//	int kek = 1;
		//for (int i = 0; i < size - 1; i++)
		//{
		//	double dist = euclidian(v[i]->c[0], v[i]->c[1], coord[size - 1][0], coord[size - 1][1]);
		//	if (dist <neighborhood)
		//	{
		//		if (v[i]->cluster!=NULL || v[size-1]->cluster!=NULL )
		//		{
		//			weight* w = new weight;
		//			weight* r = new weight;
		//			w->link = v[size - 1];
		//			r->link = v[i];
		//			w->w = dist;
		//			if (v[i]->cluster != NULL)
		//			{
		//				w->link->cluster = v[i]->cluster;
		//				w->link->pos = graphs[v[i]->cluster].size();
		//			}
		//			else
		//			{
		//				r->link->cluster = v[size - 1]->cluster;
		//				r->link->pos = graphs[v[size-1]->cluster].size();
		//			}
		//			graphs[v[i]->cluster].resize(graphs[v[i]->cluster].size()+1);
		//			//graphs[v[i]->cluster][v[size-1]->pos].resize(graphs[v[i]->cluster][v[size-1]->pos].size() + 1);
		//			r->w = dist;
		//			graphs[v[i]->cluster][v[i]->pos].push_back(w);
		//			graphs[v[i]->cluster][v[size-1]->pos].push_back(r);
		//		}
		//		else
		//		{
		//			/*vector<vector<weight*>> graph(2);*/
		//			weight* w = new weight;
		//			weight* r = new weight;
		//			w->link = v[size - 1];
		//			r->link = v[i];
		//			w->w = dist;
		//			r->w = dist;
		//			r->link->cluster = w->link->cluster = graphs.size();
		//			/*graph[0].push_back(w);
		//			graph[1].push_back(r);*/
		//			r->link->pos = 0;
		//			w->link->pos = 1;
		//			graphs.resize(graphs.size() + 1);
		//			graphs[graphs.size() - 1].resize(2);
		//			graphs[graphs.size() - 1][0].push_back(w);
		//			graphs[graphs.size() - 1][1].push_back(r);
		//		}
		//		break;
		//	}
		//}
 	}
	size_x += 20, size_y += 20;
	size_x = size_y = max(size_x, size_y);
	for (int i = 0; i < v.size(); i++)
	{
		if (v[i]->color == "white")
		{
			graphs.resize(graphs.size() + 1);
			dfs(v[i]);
		}
	}
	//opencv();
	//opencv(graphs);
	//vector<vector<double>> centroids = kmeans(3, 0);
	std::system("pause");
	return 0;
}