/*
ProjectName: ����ս��
Author: ����С���
Blog: http://www.cnblogs.com/msxh/
Github:https://github.com/XINCGer
Date: 2016/04/01
*/

#ifndef _FKSprite_H_
#define _FKSprite_H_

#include "cocos2d.h"
#include "ui\CocosGUI.h"
USING_NS_CC;
using namespace ui;

//�Զ��徫����
class FKSprite :public Sprite{
private:
	//����ֵ
	int _lifeValue;
	//Ѫ��
	LoadingBar * _HP;
	//Ѫ���ĸ�����(ÿ���ӵ�����һ�μ��ٵ�Ѫ��)
	float _HPInterval;
public:
	//��̬��create����
	static FKSprite *createWithSpriteFrameName(const std::string &filename){
		FKSprite *sprite = new FKSprite();
		if (sprite &&sprite->initWithSpriteFrameName(filename)){
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}
	void setLifeValue(int lifeValue){
		_lifeValue = lifeValue;
	}
	int getLifeValue(){
		return _lifeValue;
	}
	void setHP(LoadingBar *HP){
		_HP = HP;
	}
	LoadingBar *getHP(){
		return _HP;
	}
	void setHPInterval(float HPInterval){
		_HPInterval = HPInterval;
	}
	float getHPInterval(){
		return _HPInterval;
	}
};
#endif