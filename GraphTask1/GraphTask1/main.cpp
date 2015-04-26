/*
* Copyright 2014.215B工作室
* All rights reserved.
*
* 文件名称：main.cpp
* 摘	要：图论大作业
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
#include <iostream>
#include <fstream>
#include <cmath>
#include <clocale>
using namespace std;

void Tip();
bool Choose(std::ostream& output, Graph& g, int& times);

int main(int argc, char* args[])
{
	Tip();
	Graph g;
	ifstream input("result.txt");
	Load(input, g);
	input.close();
	cout << "Load Successfully!\n"
		"The Network contains " << g.m_v << " points and " << g.m_e << " edges.\n"; 
	for(int i = 0;i < g.m_e;i++)
		if(g.m_r[i] != 100)
			g.m_r[i] = -log(g.m_r[i] / 100) * 100;
		else
			g.m_r[i] = 0;
	//Graph g_MinPath(g);
	//int gate = 20;
	//int num = 0;
	//for(int i = 0;i < g_MinPath.m_v;i++)
	//	if(g_MinPath.m_p[i+1]-g_MinPath.m_p[i] > gate)
	//	{
	//		g_MinPath.m_hash[i] = true;
	//		g_MinPath.m_switch[i] = num;
	//		num++;
	//	}
	//	else
	//		g_MinPath.m_hash[i] = false;
	//Graph g_MST(g_MinPath);
	//Output_Data(g_MST);

	ofstream log("log.txt");
	int times = 0;
	while(1)
	{
		if(!Choose(log, g, times))
			break;
	}
	log.close();
}

void Tip()
{
	cout << "Project:Analysis on Music_Douban Network.\n"
		"Author:陈少滨(chendaxixi) 池婧雯\n"
		"Introduce:\n\tTo get data on Music_Douban and Try a Network Model."
		"\tWork out its MinPath and MST to find something interesting.\n"
		"\tBy this exe you can Enquire MinPath between Two Points and MST\n"
		"Now Load Data...Please Wait...\n";
}

bool Choose(std::ostream& output, Graph& g, int& times)
{
	cout << "Now make you choice:\n"
		"\t0. MinPath(Please Input Three Integers:0 src dst)\n"
		"\t\ttips:range from 0 to " << g.m_v-1 << "\n"
		"\t1. MST(Please Input 1)\n"
		"\tother. exit\n";
	int choose = -1;
	cin >> choose;
	if(choose == 0)
	{
		int s, t;
		cin >> s >> t;
		if((s >= g.m_v) || (s < 0) || (t >= g.m_v) || (t < 0))
		{
			cout << "Range Error!\n";
			return true;
		}
		Path p;
		Enquire_MinPath(s, t, g, p);
		cout << "MinPath between " << s << " and " << t << " is:\n";
		p.Show(cout);
		times++;
		output << "Inquiry " << times << " :\n\ttype:MinPath input: " << s << " " << t << "\n\tresult:" << 
			"MinPath between " << s << " and " << t << " is:\n";
		p.Show(output, g);
		output << "\n";
	}
	else if(choose == 1)
	{
		Tree tree;
		Enquire_MST(g, tree);
		cout << "MST is describee by edges in DFS:\n";
		tree.Show(cout, g);
		times++;
		output << "Inquiry " << times << " :\n\ttype:MST\n\tresult: tips:MST is describee by edges in DFS:\n";
		tree._Show(output, g);
		output << "\n";
	}
	else
		return false;
	return true;
}