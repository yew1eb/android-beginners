/*
ProjectName: ����ս��
Author: ����С���
Blog: http://www.cnblogs.com/msxh/
Github:https://github.com/XINCGer
Date: 2016/04/01
*/

//GameMenu�����������ʾһ���˵����������û�ѡ�񡰿�ʼ��Ϸ�����ߡ���Ϸ���á�
#ifndef _GameMenu_H_
#define _GameMenu_H_

#include "cocos2d.h"
#include "AudioControlScene.h"
using namespace cocos2d;
USING_NS_CC;

class GameMenu : public Layer{
public:
	static cocos2d::Scene *createScene();
	virtual bool init();
	CREATE_FUNC(GameMenu);
};

#endif