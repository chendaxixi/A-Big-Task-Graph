/*
* Copyright 2014.215B工作室
* All rights reserved.
*
* 文件名称：task.h
* 摘	要：API声明
*
* 当前版本：1.0
* 作	者: chendaxixi/陈少滨
* 完成日期：2015年1月5日
*
* 取代版本：1.0
* 原作者  ：chendaxixi/陈少滨
* 完成日期：2015年1月5日
*/

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>

#define NULL 0
using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::ofstream;

#ifndef TASK_H
#define TASK_H

class Graph
{
public:
	Graph(int v = 0, int e = 0):m_v(v), m_e(e), m_p(NULL), m_q(NULL), m_r(NULL), m_name(NULL), m_hash(NULL), m_switch(NULL){
		Reserve(v, e);
	}
	Graph(Graph& g){
		Reserve(g.m_v, g.m_e);
		for(int i = 0;i < m_v;i++)
		{
			m_p[i] = g.m_p[i];
			m_name[i] = g.m_name[i];
			m_hash[i] = g.m_hash[i];
			m_switch[i] = g.m_switch[i];
		}
		m_p[m_v] = g.m_p[g.m_v];
		for(int i = 0;i < m_e;i++)
		{
			m_q[i] = g.m_q[i];
			m_r[i] = g.m_r[i];
		}
	}
public:
	void Reserve(int v, int e){
		m_v = v;
		m_e = e;
		if(m_p != NULL) delete[] m_p;
		if(m_q != NULL) delete[] m_q;
		if(m_r != NULL) delete[] m_r;
		if(m_name != NULL) delete[] m_name;
		m_p = new int[m_v+1];
		m_q = new int[m_e];
		m_r = new double[m_e];
		m_name = new string[m_v];
		m_hash = new bool[m_v];
		m_switch = new int[m_v];
	}
public:
	int m_v;
	int m_e;
	int* m_p;
	int* m_q;
	double* m_r;
	string* m_name;
	bool* m_hash;
	int* m_switch;
};

class Path
{
public:
	Path():m_path(NULL), m_epath(NULL), m_weight(0){
	}
public:
	void Show(std::ostream& output)
	{
		if(m_path == NULL)
			output << "No Path.\n";
		else
		{
			output << "Path: ";
			int* s = m_path;
			while(*s != -1)
			{
				output << *s << " -> ";
				s++;
			}
			output << "end\n"
				"Weights: " << m_weight << "\n";
		}
	}
	void Show(std::ostream& output, Graph& g)
	{
		if(m_path == NULL)
			output << "No Path.\n";
		else
		{
			output << "Path: ";
			int* s = m_path;
			while(*s != -1)
			{
				output << g.m_name[*s] << " -> ";
				s++;
			}
			output << "end\n"
				"Weights: " << m_weight << " semblance: " << exp((-m_weight/100)) << "\n";
		}
	}
public:
	int* m_path;
	double m_weight;
	int* m_epath;
};

class Tree
{
public:
	Tree(int id = 0):m_id(id), m_brother(NULL), m_son(NULL), m_weight(-1){
	}
public:
	Tree* Find(int id)
	{
		if(m_id == id)
			return this;
		Tree* son = m_son;
		Tree* temp = NULL;
		while(son != NULL)
		{
			temp = son->Find(id);
			if(temp != NULL)
				return temp;
			son = son->m_brother;
		}
		return NULL;
	}
	void Show(std::ostream& output, Graph& g)
	{
		if(m_son == NULL)
			return;
		Tree* son = m_son;
		while(son != NULL)
		{
			output << m_id << " " << son->m_id << " " << g.m_r[m_weight] << "\n";
			son->Show(output, g);
			son = son->m_brother;
		}
	}
	void _Show(std::ostream& output, Graph& g)
	{
		if(m_son == NULL)
			return;
		Tree* son = m_son;
		while(son != NULL)
		{
			output << g.m_name[m_id] << " " << g.m_name[son->m_id] << " " << g.m_r[m_weight] << " " << exp((-g.m_r[m_weight]/100)) << "\n";
			son->_Show(output, g);
			son = son->m_brother;
		}
	}
public:
	Tree* m_brother;
	Tree* m_son;
	int m_id;
	int m_weight;
};

#endif

void Load(std::istream& input, Graph& g);
void Enquire_MinPath(int src, int dst, Graph& g, Path& path);
void Enquire_MST(Graph& g, Tree& tree);
void Output_Data(Graph& g);

int Find(vector<string>& name, string& str);
void Relax(int* p, int* q, double* w, int s, int* pre, int* preW, double* d);
void Update(Graph& g, bool* flag, int s, double* d, int* q, int* src);
void TreeShow(std::ostream& output, Tree& node, int* switch_);
void IntToStr(int num, char* s);
void Add(vector<vector<int>>& path, vector<vector<int>>& epath, Path& p);
int Find(vector<vector<int>>& path, vector<vector<int>>& epath, Path& p);
void PathToStr(Graph& g, vector<int>& epath, int* switch_, char* s);
void PathToStr(Graph& g, vector<int>& path, char* s);
void FindLeaf(std::ostream& output, Tree& node, Graph& g);