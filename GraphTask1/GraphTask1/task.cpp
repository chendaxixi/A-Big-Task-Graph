/*
* Copyright 2014.215B工作室
* All rights reserved.
*
* 文件名称：task.cpp
* 摘	要：API定义
*
* 当前版本：1.0
* 作	者: chendaxixi/陈少滨
* 完成日期：2015年1月5日
*
* 取代版本：1.0
* 原作者  ：chendaxixi/陈少滨
* 完成日期：2015年1月5日
*/

#include "task.h"

void Load(std::istream& input, Graph& g)
{
	vector<string> name;
	char s[100];
	int pos = -1;
	vector<int> src;
	vector<int> dst;
	while(true)
	{
		input.getline(s, 100);
		if(input.eof()) break;
		pos = Find(name, string(s));
		for(int i = 0;i < 5;i++)
			src.push_back(pos);
		for(int i = 0;i < 5;i++)
		{
			input.getline(s, 100);
			pos = Find(name, string(s));
			dst.push_back(pos);
		}
		input.getline(s, 100);
		if(input.eof()) break;
	}

	int n = name.size();
	int* all = new int[n];
	for(int i = 0;i < n;i++)
		all[i] = 0;
	int** a = new int*[n];
	for(int i = 0;i < n;i++)
		a[i] = new int[n];
	for(int i = 0;i < n;i++)
		for(int j = 0;j < n;j++)
			a[i][j] = 0;

	int e = src.size();
	for(int i = 0;i < e;i++)
	{
		a[src[i]][dst[i]]++;
		all[src[i]]++;
	}
	for(int i = 0;i < n;i++)
		all[i] /= 5;

	double** w = new double*[n];
	for(int i = 0;i < n;i++)
		w[i] = new double[n];
	for(int i = 0;i < n;i++)
		for(int j = 0;j < n;j++)
		{
			if(all[i] == 0)
				all[i] = 1;
			if(all[j] == 0)
				all[j] = 1;
			w[i][j] = (double)(a[i][j] + a[j][i]) * 100 / (double)(all[i] + all[j]);
		}

	e = 0;
	for(int i = 0;i < n;i++)
		for(int j = 0;j < n;j++)
			if(w[i][j] > 0)
				e++;
	g.Reserve(n, e);
	e = 0;

	for(int i = 0;i < n;i++)
	{
		g.m_name[i] = name[i];
		g.m_hash[i] = true;
		g.m_switch[i] = i;
		g.m_p[i] = e;
		for(int j = 0;j < n;j++)
			if(w[i][j] > 0)
			{
				g.m_q[e] = j;
				g.m_r[e] = w[i][j];
				e++;
			}
	}
	g.m_p[n] = e;
	delete[] all;
	for(int i = 0;i < n;i++)
	{
		delete[] a[i];
		delete[] w[i];
	}
	delete[] a;
	delete[] w;
}

void Enquire_MinPath(int src, int dst, Graph& g, Path& path)
{
	double* d = new double[g.m_v];	//d[]距离数组，d[i]表示i到s的最短路长度
	int* pre = new int[g.m_v];  //pre[]前驱数组，pre[i]表示i到s的最短路径中i的前驱
	int* preW = new int[g.m_v]; //preW[]权值数组，preW[i]表示pre[i]->i的边的标号
	bool* flag = new bool[g.m_v]; //flag[]标记数组，flag[i]==true表示i在点集S中
	for(int i = 0;i < g.m_v;i++)
	{
		d[i] = -1;	//由于不清楚w[i]范围，故用d[i]=-1表示i到s距离为无穷
		pre[i] = src;
		preW[i] = -1;
		flag[i] = false;
	}
	d[src] = 0;
	flag[src] = true;
	Relax(g.m_p, g.m_q, g.m_r, src, pre, preW, d);
	int minX = -1; //d[i]最小值的下标
	for(int i = 0;i < g.m_v-1;i++)
	{
		//选取在点集S中d[]min，并更新
		minX = -1;
		for(int j = 0;j < g.m_v;j++)
			if(g.m_hash[j])
			if(!flag[j])
			{
				if(minX == -1)
					minX = j;
				else if(d[j] != -1)
				{
					if(d[minX] == -1)
						minX = j;
					else if(d[j] < d[minX])
						minX = j;
				}
			}
		if(minX == -1) break;
		flag[minX] = true;
		if(flag[dst])
			break;
		Relax(g.m_p , g.m_q, g.m_r, minX, pre, preW, d);
	}
	//道路不通
	if(d[dst] == -1)
	{
		delete[] d;
		delete[] pre;
		delete[] flag;
		delete[] preW;
		return;
	}
	int* _path = new int[g.m_v+1];	//路径数组
	int pathLen = 0;	//路径长度
	double pathW = 0;	//路径权值
	//根据pre,preW数组递归构造_path数组
	path.m_epath = new int[g.m_v];
	pathLen = 1;
	_path[1] = dst;
	while(dst != src)
	{
		pathW += g.m_r[preW[dst]];
		path.m_epath[pathLen-1] = preW[dst];
		dst = pre[dst];
		pathLen++;
		_path[pathLen] = dst;
	}
	path.m_epath[pathLen-1] = -1;
	int temp = 0;
	for(int i = 1;i <= pathLen / 2;i++)
	{
		temp = _path[i];
		_path[i] = _path[pathLen - i + 1];
		_path[pathLen - i + 1] = temp;
	}
	_path[pathLen+1] = -1;
	delete[] d;
	delete[] pre;
	delete[] preW;
	delete[] flag;

	path.m_path = new int[pathLen];
	for(int i = 0;i <= pathLen;i++)
		path.m_path[i] = _path[i+1];
	path.m_weight = pathW;
	delete[] _path;
}

void Enquire_MST(Graph& g, Tree& tree)
{
	bool* flag = new bool[g.m_v];
	double* d = new double[g.m_v];
	int* q = new int[g.m_v];
	int* src = new int[g.m_v];
	//初始化flag, d ,q, src
	for(int i = 0;i < g.m_v;i++)
	{
		flag[i] = false;
		d[i] = INT_MAX;
		q[i] = -1;
		src[i] = -1;
	}
	int s = 0;
	for(int i = 0;i < g.m_v;i++)
		if(g.m_hash[i])
		{
			s = i;break;
		}
	//将点0加入点集V
	Update(g, flag, s, d, q, src);
	tree.m_id = s;
	double min = 0;
	Tree* temp = NULL;
	Tree* node = NULL;
	for(int i = 1;i < g.m_v;i++)
	{
		//选出V'中离V最近的点s，边用q[s]表示
		min = INT_MAX;
		s = -1;
		for(int j = 0;j < g.m_v;j++)
			if(g.m_hash[j])
			if(!flag[j])
				if(d[j] < min)
				{
					min = d[j];
					s = j;
				}
		if(s == -1) break;
		temp = tree.Find(src[s]);
		node = new Tree(s);
		node->m_weight = q[s];
		if(temp->m_son == NULL)
			temp->m_son = node;
		else
		{
			node->m_brother = temp->m_son;
			temp->m_son = node;
		}
		Update(g, flag, s, d, q, src);
	}
}

int Find(vector<string>& name, string& str)
{
	int n = name.size();
	for(int i = 0;i < n;i++)
		if(str == name[i])
			return i;
	name.push_back(str);
	return n;
}

void Relax(int* p, int* q, double* w, int s, int* pre, int* preW, double* d)
{
	if(d[s] == -1) return;
	for(int i = p[s];i < p[s+1];i++)
		if((d[q[i]] == -1) || (d[s] + w[i] < d[q[i]]))
		{
			d[q[i]] = d[s] + w[i];
			pre[q[i]] = s;
			preW[q[i]] = i;
		}
}

void Update(Graph& g, bool* flag, int s, double* d, int* q, int* src)
{
	flag[s] = true;
	for(int i = g.m_p[s];i < g.m_p[s+1];i++)
		if(!flag[g.m_q[i]])
			if(g.m_r[i] < d[g.m_q[i]])
			{
				d[g.m_q[i]] = g.m_r[i];
				q[g.m_q[i]] = i;
				src[g.m_q[i]] = s;
			}
}

void Output_Data(Graph& g)
{
	srand((unsigned)time(NULL));
	ofstream output("MusicDoubanGraph.js");
	output << "var graph = {\n"
		" \"nodes\":[\n";
	int temp = 0;
	for(int i = 0;i < g.m_v;i++)
		if(g.m_hash[i])
	{
		temp = rand() % 19 + 1;
		output << "  {\"name\":\"" << g.m_name[i] 
		<< "\",\"group\":" << temp << "},\n";
	}
	output << "],\n\"links\":[\n";

	int num = 0;
	int* switch_ = new int[g.m_e];
	for(int i = 0;i < g.m_v;i++)
		if(g.m_hash[i])
		for(int j = g.m_p[i];j < g.m_p[i+1];j++)
			if(g.m_hash[g.m_q[j]])
			{
				switch_[j] = num;
				num++;
			output << "  {\"source\":" << g.m_switch[i] <<
			",\"target\":" << g.m_switch[g.m_q[j]] << 
			",\"value\":" << (int)(g.m_r[j] / (double)30)+2 << "},\n";
			}
	//output << "],\n\"minpath\":[\n";
	//for(int i = 0;i < g.m_v;i++)
	//	if(g.m_hash[i])
	//	{
	//		vector<vector<int>> path;
	//		vector<vector<int>> epath;
	//		for(int j = 0;j < g.m_v;j++)
	//			if((i != j) && (g.m_hash[j]))
	//			{
	//				Path p;
	//				Enquire_MinPath(i, j, g, p);
	//				Add(path, epath, p);
	//			}

	//		int n = path.size();
	//		char s[500];
	//		IntToStr(g.m_switch[i], s);
	//		char* ss = s;
	//		while(*ss != 0)ss++;
	//		*ss = ' ';
	//		ss++;
	//		*ss = 0;
	//		for(int j = 0;j < n;j++)
	//		{
	//			PathToStr(g, epath[j], switch_,  ss);
	//			output << "  {\"epath\":\"" << s << "\",\"path\":\"";
	//			*s = 0;
	//			PathToStr(g, path[j], s);
	//			output << s << "\"},\n";
	//		}
	//	}
	output << "],\n\"mst_edge\":[\n";
	Tree tree;
	Enquire_MST(g, tree);
	TreeShow(output, tree, switch_);
	output << "],\n\"mst_leaf\":[\n";
	FindLeaf(output, tree, g);
	output << " ]\n}";
}

void TreeShow(std::ostream& output, Tree& node, int* switch_)
{
	if(node.m_son == NULL)
		return;
	Tree* son = node.m_son;
	while(son != NULL)
	{
		output << "  {\"edge\":" << switch_[son->m_weight] << "},\n";
		TreeShow(output, *son, switch_);
		son = son->m_brother;
	}
}

void IntToStr(int num, char* s)
{
	if(num == 0)
	{
		s[0] = '0';
		s[1] = 0;
		return;
	}
	int len = 0;
	while(num > 0)
	{
		s[len] = '0' + num % 10;
		len++;
		num /= 10;
	}
	int temp = 0;
	for(int i = 0;i < len/2;i++)
	{
		temp = s[i];
		s[i] = s[len-1-i];
		s[len-1-i] = temp;
	}
	s[len] = 0;
}

void Add(vector<vector<int>>& path, vector<vector<int>>& epath, Path& p)
{
	if(p.m_path == NULL) return;
	int pos = Find(path, epath, p);
	if(pos == -1)
	{
		vector<int> temp;
		int* s = p.m_path;
		while(*s != -1)
		{
			temp.push_back(*s);
			s++;
		}
		path.push_back(temp);

		temp.clear();
		s = p.m_epath;
		while(*s != -1)
		{
			temp.push_back(*s);
			s++;
		}
		epath.push_back(temp);
	}
}

int Find(vector<vector<int>>& path, vector<vector<int>>& epath, Path& p)
{
	int n = path.size();
	for(int i = 0;i < n;i++)
	{
		int len = 0;
		int m = path[i].size();
		while(1)
		{
			if(p.m_path[len] == -1) break;
			if(len >= m) break;
			if(p.m_path[len] != path[i][len]) break;
			len++;
		}
		if(len >= m)
		{
			while(1)
			{
				path[i].push_back(p.m_path[len]);
				len++;
				if(p.m_path[len] == -1) break;
			}

			epath[i].clear();
			int* s = p.m_epath;
			vector<int> temp;
			while(*s != -1)
			{
				temp.push_back(*s);
				s++;
			}
			epath[i] = temp;
			return i;
		}
		else if (p.m_path[len] == -1) 
			return i;
	}
	return -1;
}

void PathToStr(Graph& g, vector<int>& epath, int* switch_, char* s)
{
	int n = epath.size();
	for(int i = 0;i < n;i++)
	{
		IntToStr(switch_[epath[i]], s);
		while(*s != 0)
			s++;
		*s = ' ';
		s++;
		*s = 0;
	}
}

void PathToStr(Graph& g, vector<int>& path, char* s)
{
	int n = path.size();
	for(int i = 0;i < n;i++)
	{
		IntToStr(g.m_switch[path[i]], s);
		while(*s != 0)
			s++;
		*s = ' ';
		s++;
		*s = 0;
	}
}

void FindLeaf(std::ostream& output, Tree& node, Graph& g)
{
	if((node.m_son == NULL) || (node.m_weight == -1))
		output << "  {\"leaf\":" << g.m_switch[node.m_id] << "},\n";
	Tree* son = node.m_son;
	while(son != NULL)
	{
		FindLeaf(output, *son, g);
		son = son->m_brother;
	}
}