#pragma once
#include "cocos2d.h"
#include <map>
#include <queue>

class Node
{
public:
	cocos2d::Point m_pos;					//��ǰλ��
	int m_curToBeginDistance = 0;	//��ǰλ���뿪ʼλ�õľ���G
	int m_curToEndDistance = 0;		//��ǰλ����Ŀ�����С����H
	int m_f = 0;									//Fֵ = G + H
	bool m_isWall = false;					//�ǲ����ϰ���
	::Node* m_parentNode = nullptr;	//���ڵ�
	bool m_isInOpenQueue = false;				//�Ƿ���open������
	bool m_isInCloseVec = false;					//�Ƿ���closeVec��
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
	//�������·��
	std::vector<cocos2d::Point> findShortPath(cocos2d::Point beginPoint, cocos2d::Point endPoint);
	//��ȡһ���ڵ����ڵĽڵ�
	std::vector<::Node*> getAdjacentNode(const Node * node, const Node * endNode);
	//����ڵ��GHFֵ
	void calculateGHFofNode(::Node* node, const ::Node* beginNode, const ::Node* endNode);
	std::map<cocos2d::Point, ::Node*> m_allNode;
	void reset();

private:
	int abs(int num);
	//��������
	std::priority_queue < ::Node*,std::vector<Node*>, cmp > m_openQueue;
	//�رն���
	std::vector<::Node*> m_closeVec;
};