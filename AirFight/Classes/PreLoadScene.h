/*
ProjectName: ����ս��
Author: ����С���
Blog: http://www.cnblogs.com/msxh/
Github:https://github.com/XINCGer
Date: 2016/04/01
*/

//Ԥ�����࣬���Ԥ���ر������֡���Ч���������׼������

#ifndef _PreLoadScene_H_
#define _PreLoadScene_H_

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"

USING_NS_CC;
using namespace CocosDenshion;

class PreLoad : public Layer{
public:
	static Scene * createScene();
	virtual bool init();
	//��Node�ڵ���롰��̨��ʱ����
	virtual void onEnterTransitionDidFinish();
	//���ر�������
	void loadMusic(ValueVector musicFiles);
	//������Ϸ��Ч
	void loadEffect(ValueVector effectFiles);
	//���ؾ����
	void loadSpriteSheets(ValueVector spriteSheets);
	//������Ϸ������
	void progressUpdate();
	CREATE_FUNC(PreLoad);
protected:
	//����������Ϸ��Ҫ���ص���Դ����
	int _sourceCount;
	//������
	ProgressTimer * _progress;
	//���������µĴ���
	float _progressInterval;
};

#endif