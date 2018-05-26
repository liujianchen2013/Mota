#include "AutoFindPath.h"

AutoFindPath::~AutoFindPath()
{
	for (auto& it : m_allNode)
	{
		if (it.second)
		{
			delete it.second;
			it.second = nullptr;
		}
	}
}

std::vector<cocos2d::Point> AutoFindPath::findShortPath(cocos2d::Point beginPoint, cocos2d::Point endPoint)
{
	Node *beginNode = m_allNode.at(beginPoint);
	Node *endNode = m_allNode.at(endPoint);
	if (beginNode == nullptr || endNode == nullptr)
	{
		return std::vector<cocos2d::Point>();
	}
	//�Ƿ��ҵ�·��
	bool bHavePath = false;
	//��ǰ�ڵ�
	Node* curNode = nullptr;
	m_openQueue.push(beginNode);

	while (!m_openQueue.empty() && !bHavePath)
	{
		//��ȡһ��Fֵ��С�Ľڵ�����Ϊ��ǰ�ڵ�
		curNode = m_openQueue.top();
		//���ýڵ��open�����е���
		m_openQueue.pop();
		curNode->m_isInOpenQueue = false;
		//����ǰ�ڵ�ѹ��close������
		m_closeVec.push_back(curNode);
		curNode->m_isInCloseVec = true;

		//��ȡ��ǰ�ڵ����ڵ��ҺϷ��Ľڵ�(û��Խ��,����Wall,����openQueue��)
		std::vector<Node*> adjacentsNode = getAdjacentNode(curNode,endNode);

		for (auto const& it : adjacentsNode)
		{
			calculateGHFofNode(it, beginNode, endNode);
			it->m_parentNode = curNode;
			if (it == endNode)
			{
				bHavePath = true;
				break;
			}
			m_openQueue.push(it);
			it->m_isInOpenQueue = true;
		}
	}

	std::vector<cocos2d::Point> shortPath;
	if (bHavePath)
	{
		for (Node* node = endNode; node != nullptr; node = node->m_parentNode)
		{
			shortPath.push_back(node->m_pos);
		}
	}
	reset();
	return shortPath;
}

std::vector<Node*> AutoFindPath::getAdjacentNode(const Node * node, const Node * endNode)
{
	std::vector<Node*> checkNode;
	if (node)
	{
		cocos2d::Point points[4];
		points[0] = node->m_pos + cocos2d::Point(0, -1);
		points[1] = node->m_pos + cocos2d::Point(-1, 0);
		points[2] = node->m_pos + cocos2d::Point(0, 1);
		points[3] = node->m_pos + cocos2d::Point(1,0);

		for (auto const& it : points)
		{
			auto item = m_allNode.find(it);
			//�ýڵ����
			if (item != m_allNode.end())
			{
				Node* tmp = item->second;
				//�ýڵ㲻��Wall && ����openQueue�� && ����closeVec��
				if (tmp && (!tmp->m_isWall || tmp == endNode) && !tmp->m_isInOpenQueue && !tmp->m_isInCloseVec)
				{
					checkNode.push_back(tmp);
				}
			}
		}
	}
	return checkNode;
}

void AutoFindPath::calculateGHFofNode(Node * node, const Node * beginNode, const Node * endNode)
{
	if (node && beginNode && endNode)
	{
		int gValue = abs(abs(node->m_pos.x) - abs(beginNode->m_pos.x)) + abs(abs(node->m_pos.y) - abs(beginNode->m_pos.y));
		int hValue = abs(abs(node->m_pos.x) - abs(endNode->m_pos.x)) + abs(abs(node->m_pos.y) - abs(endNode->m_pos.y));

		node->m_curToBeginDistance = gValue;
		node->m_curToEndDistance = hValue;
		node->m_f = gValue + hValue;
	}
}

void AutoFindPath::reset()
{
	while (!m_openQueue.empty())
	{
		m_openQueue.pop();
	}
	m_closeVec.clear();

	for (auto& it : m_allNode)
	{
		Node* node = it.second;
		if (node)
		{
			node->reset();
		}
	}
}

int AutoFindPath::abs(int num)
{
	if (num < 0)
		return 0 - num;
	return num;
}

void Node::reset()
{
	m_curToBeginDistance = 0;
	m_curToEndDistance = 0;
	m_f = 0;
	m_parentNode = nullptr;
	m_isInOpenQueue = false;
	m_isInCloseVec = false;
}

bool Node::operator>(const Node & b)
{
	return m_f > b.m_f;
}
