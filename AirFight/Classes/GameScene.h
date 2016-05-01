/*
ProjectName: ����ս��
Author: ����С���
Blog: http://www.cnblogs.com/msxh/
Github:https://github.com/XINCGer
Date: 2016/04/01
*/

#ifndef _GameScene_H_
#define _GameScene_H_
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "FKSprite.h"
using namespace cocos2d;
using namespace CocosDenshion;

namespace my{
	class Game : public Layer{
	private:
		//��ҷɻ�����
		Sprite * _plane;
		//��Ļ�Ŀ�ȡ��߶ȱ���
		int _screenWidth, _screenHeight;
		//��Ϸ����ͼ
		Sprite *_bg1;
		Sprite *_bg2;
		//��Ϸ֡������
		int _count;
		//�л�����
		Vector<FKSprite *> _enemyVector;
		//�ӵ�����
		Vector<Sprite *> _bulletVector;
		//����ֵ��ǩ
		Label * _scoreLabel;
		//����
		int _scoreValue;
		//��BOSS
		FKSprite * _boss =NULL;
		//��BOSS�ڵ���������
		Vector <Sprite*> _bossBulletVecor;
		//�Ƿ�������BOSS����BOSS�Ƿ�ʼ�ƶ��ʹ�BOSS�Ƿ����ӵ�
		bool isStart=false, _isMove=false, isShoot=false;

	public:
		static Scene *createScene();
		virtual bool init();
		//���ڵ����ڵĳ����������ֵ�ʱ�����
		virtual void onEnter();
		// ��ö�������
		Animation* getAnimationByName(std::string animName, float delay, int animNum);
		//�ص����º������ú���ÿһ֡���ᱻ����
		void update(float delta);
		//����ͼƬ��������
		void updateBackground();
		//���µл�����
		void enemyPlaneDive(float delta);
		//�л��뿪��Ļɾ������
		void removeEnemy(float delta);
		//��ҷɻ����Ϸ����ӵ��ĺ���
		void shootBullet(float delta);
		//�ӵ��뿪��Ļɾ���ĺ���
		void removeBullet(float delta);
		//�����ײ�ĺ���
		void collisionDetection(float delta);
		//���ű�ը����
		void playBombAnimate(std::string name, Vec2 position);
		//��Ϸ����
		void gameOver(std::string message);
		//���¿�ʼ
		void restart(int tag);
		//������BOSS
		void startBoss();
		//�ƶ���BOSS
		void moveBoss();
		//��BOSS�����ڵ�
		void shootBoss(float delta);
		//�������´�Boss��Ѫ��
		void updateHUD(float delta);
		CREATE_FUNC(my::Game);
	};
}

#endif