#pragma once
#include "cocos2d.h"
#include <map>
#include <queue>

class Node
{
public:
	cocos2d::Point m_pos;					//当前位置
	int m_curToBeginDistance = 0;	//当前位置离开始位置的距离G
	int m_curToEndDistance = 0;		//当前位置离目标的最小距离H
	int m_f = 0;									//F值 = G + H
	bool m_isWall = false;					//是不是障碍物
	::Node* m_parentNode = nullptr;	//父节点
	bool m_isInOpenQueue = false;				//是否在open队列中
	bool m_isInCloseVec = false;					//是否在closeVec中
	void reset();
	bool operator>(const Node& b);
};

struct cmp 
{
	bool operator()(Node* a, Node* b)
	{
		return a->m_f > b->m_f;
	}
};

class AutoFindPath
{
public:
	~AutoFindPath();
	//查找最短路线
	std::vector<cocos2d::Point> findShortPath(cocos2d::Point beginPoint, cocos2d::Point endPoint);
	//获取一个节点相邻的节点
	std::vector<::Node*> getAdjacentNode(const Node * node, const Node * endNode);
	//计算节点的GHF值
	void calculateGHFofNode(::Node* node, const ::Node* beginNode, const ::Node* endNode);
	std::map<cocos2d::Point, ::Node*> m_allNode;
	void reset();

private:
	int abs(int num);
	//开启队列
	std::priority_queue < ::Node*,std::vector<Node*>, cmp > m_openQueue;
	//关闭队列
	std::vector<::Node*> m_closeVec;
};