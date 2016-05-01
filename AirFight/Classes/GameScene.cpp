/*
ProjectName: ����ս��
Author: ����С���
Blog: http://www.cnblogs.com/msxh/
Github:https://github.com/XINCGer
Date: 2016/04/01
*/

#include "GameScene.h"
#include "MenuScene.h"
#include "ReadJson.h"
USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;

//��ͬ�л���tag
static int E0TAG = 10;
static int E1TAG = 11;
static int E2TAG = 12;
static int BOSSTAG = 100;

Scene * my::Game::createScene(){
	// ����һ���������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
	auto scene = Scene::create();
	// ��������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
	auto layer = Game::create();
	// ��Game����Ϊ�ӽڵ���ӵ�����
	scene->addChild(layer);
	// ���س�������
	return scene;
}

bool my::Game::init(){
	if (!Layer::init()){
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//��ȡ��Ļ�Ŀ����߶�
	_screenWidth = visibleSize.width;
	_screenHeight = visibleSize.height;
	//��������ӵ�����֡����
	AnimationCache::getInstance()->addAnimation(getAnimationByName("plane", 0.08, 2), "fly");
	AnimationCache::getInstance()->addAnimation(getAnimationByName("blast", 0.09, 4), "blast");
	AnimationCache::getInstance()->addAnimation(getAnimationByName("bomb", 0.1, 4), "bomb");
	return true;
}

void my::Game::onEnter(){
	Layer::onEnter();
	//��Ϸ����ͼ
	//Sprite *bg1 = Sprite::create("bg1.png");
	//bg1->setAnchorPoint(Vec2(0, 0));
	//this->addChild(bg1, -1);

	//��ʼ���������
	srand(time(NULL));
	//������������ı���������֮ǰ�ľ�̬����
	std::string bgName;
	//����Ӧ��Ļ��С
	if (_screenHeight == 960){
		bgName = "bg1.png";
	}
	else{
		bgName = "bg2.png";
	}
	//��һ�ű���ͼ
	_bg1 = Sprite::create(bgName);
	_bg1->setAnchorPoint(Vec2::ZERO);
	//setAliasTexParameters���ڽ��ƴ�ӵĵ�ͼ�����ӹ���ʱ�����γɺ�ɫ��϶������
	_bg1->getTexture()->setAliasTexParameters();
	this->addChild(_bg1);

	//�ڶ��ű���ͼ
	_bg2 = Sprite::create(bgName);
	_bg2->setAnchorPoint(Vec2::ZERO);
	_bg2->getTexture()->setAliasTexParameters();
	_bg2->setPosition(Vec2(0, _bg1->getContentSize().height));
	this->addChild(_bg2);

	//�����ҷɻ�����
	_plane = Sprite::createWithSpriteFrameName("plane0.png");
	_plane->setPosition(Vec2(_screenWidth / 2, _plane->getContentSize().height/2+5));
	this->addChild(_plane);

	//��ҷɻ�������β�����
	auto planeFlyAnimation = AnimationCache::getInstance()->getAnimation("fly");
	//������������
	auto animate = Animate::create(planeFlyAnimation);
	//�ظ�����
	auto planeFlyAction = RepeatForever::create(animate);
	//ִ�ж�����ʵ�ַɻ�β�����Ч��
	_plane->runAction(planeFlyAction);

	//�����¼�������
	auto planeListener = EventListenerTouchOneByOne::create();
	//��Ӧ�����¼�����
	planeListener->onTouchBegan = [](Touch *touch, Event *event){
		//����¼�����
		auto target = event->getCurrentTarget();
		//��ô�����Opengl����
		Vec2 location = touch->getLocation();
		//��ǰ���Event(_plane)�Ľڵ�����
		Vec2 locationInNode = target->convertToNodeSpace(location);
		Size s = target->getContentSize();
		//��ȡ�ö���ľ�������
		Rect rect = Rect(0, 0, s.width, s.height);
		//��������������������
		if (rect.containsPoint(locationInNode)){
			return true;
		}
		return false;
	};

	//��ӱ�������
	SimpleAudioEngine::getInstance()->playBackgroundMusic("s3.wav", true);

	//��ʼ��������ǩ
	_scoreLabel = Label::createWithSystemFont("00", "Arial", 32);
	_scoreLabel->setPosition(50, _screenHeight*0.98);
	this->addChild(_scoreLabel);
	//��ʼ������ֵ
	_scoreValue = 0;

	planeListener->onTouchMoved = [](Touch *touch, Event *event){
		//����¼�����
		auto target = event->getCurrentTarget();
		//�ƶ������ľ���λ��
		target->setPosition(target->getPosition() + touch->getDelta());
	};
	//��ӳ��������¼�������
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(planeListener, _plane);
	//��Ϸ��ѭ����ÿ֡�����õĺ���
	this->scheduleUpdate();
	//ָ��ÿִ֡���Զ���ĺ���
	this->schedule(schedule_selector(my::Game::shootBullet)); //�����ӵ�
	this->schedule(schedule_selector(my::Game::collisionDetection)); //�����ײ
	this->schedule(schedule_selector(my::Game::shootBoss)); //��Boss�����ӵ�
}

Animation* my::Game::getAnimationByName(std::string animName, float delay, int animNum){
	/*
	��ö�������
	animName :����֡������
	delay :����֡��֮֡��ļ��ʱ��
	animNum :����֡������
	*/
	auto animation = Animation::create();
	//ѭ���Ӿ���֡�����л�ȡ��ͼƬ�������Ӧ�ľ���֡��ɶ���
	for (unsigned int i = 0; i < animNum; i++){
		//��ȡ����ͼƬ���ƣ�����plane0.png
		std::string frameName = animName;
		frameName.append(StringUtils::format("%d", i)).append(".png");
		//������ͼƬ���Ϊ����֡��������֡��
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName.c_str()));
	}
	//���ö������ŵ�����
	animation->setDelayPerUnit(delay);
	//�þ�������ڶ���ִ�����ָ������״̬
	animation->setRestoreOriginalFrame(true);
	//���ض�������
	return animation;
}

//����ͼƬ��������
void my::Game::updateBackground(){
	//��ȡ����1ͼƬ��λ��
	Vec2 p1 = _bg1->getPosition();
	//����1ͼƬ��y��ÿ֡�Լ�5
	p1.y = p1.y - 5;
	//�������1ͼƬ��y��С�ڵ��ڸ�����1ͼƬ����ĸ߶ȣ�������1ͼƬ�Ƴ���Ļ�����򱳾�1ͼƬ��y��
	//���µ��ڱ���1ͼƬ������ĸ߶�
	if (p1.y <= -_bg1->getContentSize().height){
		p1.y = _bg1->getContentSize().height;
	}
	//�������ñ���1ͼƬ��λ��
	_bg1->setPosition(p1);
	//����2ͼƬ�Ĳ����뱳��1ͼƬ�Ĳ���һ��
	Vec2 p2 = _bg2->getPosition();
	p2.y = p2.y - 5;
	if (p2.y <= -_bg2->getContentSize().height){
		p2.y = _bg2->getContentSize().height;
	}
	_bg2->setPosition(p2);
}

void my::Game::update(float delta){
	_count++;
	updateBackground(); //����ͼƬ����
	enemyPlaneDive(delta); //�л�����
	removeEnemy(delta); //ɾ���뿪��Ļ�ĵл�
	//shootBullet(delta); //�����ӵ�
	removeBullet(delta); //ɾ���뿪��Ļ���ӵ�
	//collisionDetection(delta); //�����ײ
	updateHUD(delta);  //����HUD
}

//enemyPlaneDive�������ڿ��Ƶл��Ĵ���������
void my::Game::enemyPlaneDive(float delta){
	bool flag = false;
	int lifeValue;
	//�Զ��徫����FKSprite
	FKSprite *enemyPlane;
	//ʹ������������õл���x����
	int randX = cocos2d::random()%(_screenWidth - 40) + 20;
	//��ȡ���ʱ�䣨�л������ʱ�䣩
	float durationTime = cocos2d::random() % 2 + 2;
	//countΪ20�ı���ʱ���һ�ܵл�
	if (_count % 30 == 0 && !isStart){
		//����cocos2d::random�������䲻ͬ�ĵл�
		int rand = cocos2d::random() % 2;
		switch (rand)
		{
		case 0:
			enemyPlane = FKSprite::createWithSpriteFrameName("e0.png");
			enemyPlane->setTag(E0TAG);
			break;
		case 1:
			enemyPlane = FKSprite::createWithSpriteFrameName("e2.png");
			enemyPlane->setTag(E2TAG);
			break;
		}
		//���õл�����ֵ���л�ͼƬ����
		lifeValue = 1;
		enemyPlane->setLifeValue(lifeValue);
		enemyPlane->setScale(0.6);
		flag = true;
	}
	else{
		if (_count % 200 == 0 && !isStart){
			enemyPlane = FKSprite::createWithSpriteFrameName("e1.png");
			enemyPlane->setTag(E1TAG);
			enemyPlane->setScale(0.8);
			lifeValue = 10;
			enemyPlane->setLifeValue(lifeValue);
			flag = true;
		}
	}
	if (flag){
		//���õл���λ��
		enemyPlane->setPosition(randX, _screenHeight + enemyPlane->getContentSize().height);
		//MoveBy ����,�߶�Ϊ��Ļ�߶ȼ��Ϸɻ�����ĸ߶�
		auto moveBy = MoveBy::create(durationTime, Vec2(0, -enemyPlane->getPosition().y - enemyPlane->getContentSize().height));
		//�л����嶯��
		enemyPlane->runAction(moveBy);
		//���л�������ӵ��л�����
		_enemyVector.pushBack(enemyPlane);
		//���л����Ϊ��ǰ����ӽڵ�
		this->addChild(enemyPlane);
		//����л���һ���ɵ����һ��Ѫ��
		if (enemyPlane->getTag() == E1TAG){
			//����һ��LoadingBar
			auto loadingBar = LoadingBar::create("planeHP.png");
			loadingBar->setScale(0.15);
			//���ý������ķ���
			loadingBar->setDirection(LoadingBar::Direction::LEFT);
			//���ý������ĳ�ʼֵΪ100����Ѫ
			loadingBar->setPercent(100);
			Point pos = enemyPlane->getPosition();
			//��������λ��
			loadingBar->setPosition(Vec2(pos.x, pos.y + 48));
			//��LoadingBar���Ϊ��ǰ����ӽڵ�
			this->addChild(loadingBar, 1);
			//������ִ��moveby����
			auto moveBy2 = MoveBy::create(durationTime, Vec2(0, -pos.y - enemyPlane->getContentSize().height));
			loadingBar->runAction(moveBy2);
			//���ý���������Ϊ�л���Ѫ��
			enemyPlane->setHP(loadingBar);
			//����Ѫ��������
			enemyPlane->setHPInterval(100.0f / enemyPlane->getLifeValue());
		}
	}
}

//���л��Ѿ��Ƴ���Ļ��ʱɾ���л�����
void my::Game::removeEnemy(float delta){
	//�������еĵл�����
	for (unsigned int i = 0; i < _enemyVector.size(); i++){
		auto enemy = _enemyVector.at(i);
		//����л��Ѿ��Ƴ���Ļ�⣬ɾ���л�����
		if (enemy->getPosition().y <= -enemy->getContentSize().height){
			//�ӵ�ǰ��ɾ���õл�����
			this->removeChild(enemy, true);
			//��_enemyVector������ɾ���л�����
			_enemyVector.eraseObject(enemy);
		}
	}
}

//shootBullet�������ڲ��Ϸ����ӵ�
void my::Game::shootBullet(float delta){
	//��÷ɻ�������
	Vec2 pos = _plane->getPosition();
	//����_countΪ8�ı���ʱ����һ���ӵ�
	if (_count % 8 == 0){
		//���������ӵ��ľ���
		auto bullet = Sprite::createWithSpriteFrameName("bullet.png");
		//�����ӵ�������
		bullet->setPosition(pos.x, pos.y + _plane->getContentSize().height / 2 + bullet->getContentSize().height);
		//�ӵ��ƶ�ʱ��Ϊ0.4�룬�ƶ�����Ϊ��Ļ�߶�-�ӵ���y��
		auto moveBy = MoveBy::create(0.4f, Vec2(0, _screenHeight - bullet->getPosition().y));
		bullet->runAction(moveBy);
		//���ӵ�������ӵ���ǰ��
		this->addChild(bullet, 4);
		//���ӵ�������ӵ��ӵ���������
		_bulletVector.pushBack(bullet);
	}
}

void my::Game::removeBullet(float delta){
	//�����ӵ�����
	for (unsigned int i = 0; i < _bulletVector.size(); i++){
		auto bullet = _bulletVector.at(i);
		//����ӵ��Ѿ��Ƴ���Ļ�⣬��ɾ���ӵ�
		if (bullet->getPosition().y >= _screenHeight){
			//�ӵ�ǰ����ɾ�����ӵ�����
			this->removeChild(bullet, true);
			//���ӵ�����������ɾ���ӵ�����
			_bulletVector.eraseObject(bullet);
		}
	}
}

void my::Game::collisionDetection(float delta){
	//�����л�����
	for (unsigned int i = 0; i < _enemyVector.size(); i++){
		FKSprite *enemy = _enemyVector.at(i);
		//�ж���ҷɻ��͵л�������ײ
		if (_plane->getBoundingBox().intersectsRect(enemy->getBoundingBox())){
			//���ű�ը����
			playBombAnimate("blast", enemy->getPosition());
			playBombAnimate("blast", _plane->getPosition());
			//������Ǵ�boss,ɾ���л�����
			if (enemy->getTag() != BOSSTAG){
				_enemyVector.eraseObject(enemy);
				this->removeChild(enemy, true);
			}
			//��ҷɻ�ֹͣ���ж���
			_plane->stopAllActions();
			//��Ҿ��鲻�ɼ�
			_plane->setVisible(false);
			gameOver(ReadJson::getString("restart"));
		}
		else{
			//�����ӵ�����
			for (unsigned int i = 0; i < _bulletVector.size(); i++){
				auto bullet = _bulletVector.at(i);
				//����л����ӵ�������ײ
				if (enemy->getBoundingBox().intersectsRect(bullet->getBoundingBox())){
					//�����ӵ���Ч
					SimpleAudioEngine::getInstance()->playEffect("bullet.wav");
					//ɾ���ӵ�����
					_bulletVector.eraseObject(bullet);
					this->removeChild(bullet, true);
					//�л�����ֵ��1
					enemy->setLifeValue(enemy->getLifeValue() - 1);
					//�ɵ��ʹ�bossѪ������
					if (enemy->getTag() == E1TAG || enemy->getTag() ==BOSSTAG){
						if (enemy->getHP() != nullptr){
							enemy->getHP()->setPercent(enemy->getHPInterval()*enemy->getLifeValue());
						}
					}
					//�жϵл�����ֵ
					if (enemy->getLifeValue() <= 0){
						//����Ǵ�boss��ֹͣ��boss�����ж���
						if (enemy->getTag() == BOSSTAG){
							_boss->stopAllActions();
							//����boss��ը����
							playBombAnimate("bomb", enemy->getPosition());
							SimpleAudioEngine::getInstance()->playEffect("b1.wav");
							//���·���
							_scoreLabel->setString(StringUtils::format("%d", _scoreValue + 10000));
							gameOver(ReadJson::getString("win"));
						}
						else{
							//���ű�ը����
							playBombAnimate("blast", enemy->getPosition());
							SimpleAudioEngine::getInstance()->playEffect("b0.wav");
							//�жϷɻ��÷�
							if (enemy->getTag() == E1TAG){
								_scoreLabel->setString(StringUtils::format("%d", _scoreValue += 500));
							}
							else{
								_scoreLabel->setString(StringUtils::format("%d", _scoreValue += 100));
							}
						}

						//ɾ���л�����
						_enemyVector.eraseObject(enemy);
						this->removeChild(enemy, true);
						//10000��ʱ������Boss
						if (_scoreValue >= 10000 && ! isStart){
							startBoss();
							//��Boss����������ͨ�����øñ���ֹͣ�ɻ�����
							isStart = true;
							break;
						}
					}
					break;
				}
			}
			//�жϴ�Boss���ӵ��Ƿ����ҷɻ���ײ
			if (isShoot){
				for (unsigned int i = 0; i < _bossBulletVecor.size(); i++){
					auto bossBullet = _bossBulletVecor.at(i);
					if (_plane->getBoundingBox().intersectsCircle(Vec2(bossBullet->getPositionX(), bossBullet->getPositionY()),bossBullet->getContentSize().height/2)){
						_plane->stopAllActions();
						_plane->setVisible(false);
						playBombAnimate("blast", _plane->getPosition());
						SimpleAudioEngine::getInstance()->playEffect("b0.wav");
						gameOver(ReadJson::getString("restart"));
					}
				}
			}
		}
	}
}
/* ���ű�ը����
name: ��ը����������
position����ը�������ŵ�λ�� */
void my::Game::playBombAnimate(std::string name, Vec2 position){
	//��ñ�ը����
	Animation *blastAnimation;
	if (name.compare("bomb") == 0){
		blastAnimation = AnimationCache::getInstance()->getAnimation("bomb");
	}
	else{
		blastAnimation = AnimationCache::getInstance()->getAnimation("blast");
	}
	std::string bombName = name + "0.png";
	//����һ������
	auto blast = Sprite::createWithSpriteFrameName(bombName);
	blast->setPosition(position);
	//��϶�����1.���Ŷ���2.�����걬ը����֮��ɾ������
	auto animate = Animate::create(blastAnimation);
	auto callFunc = CallFunc::create([=]{
		this->removeChild(blast, true);
	});
	auto action = Sequence::create(animate, callFunc, NULL);
	//ִ�б�ը����
	blast->runAction(action);
	//��������ӵ������
	this->addChild(blast);

}

//gameOver����������Ϸ����ʱ������������ʾ��Ϸ��Ϣ

void my::Game::gameOver(std::string message){
	//ֹͣ�ص����º���
	//this->unscheduleUpdate();
	//ָֹͣ��ÿִ֡���Զ���ĺ����������ٷ����ӵ��������ײ�ͷ����Boss���ڵ�
	this->unschedule(schedule_selector(my::Game::shootBullet));
	this->unschedule(schedule_selector(my::Game::collisionDetection));
	this->unschedule(schedule_selector(my::Game::shootBoss));
	//���¿�ʼ��ť
	auto restart_button = Button::create("button.png");
	restart_button->setScale(2);
	restart_button->setTitleText(message);
	restart_button->setTitleFontName("΢���ź�");
	restart_button->setTitleFontSize(16);
	restart_button->setPosition(Vec2(_screenWidth / 2, _screenHeight*0.6));
	restart_button->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
		if (type == Widget::TouchEventType::ENDED){
			this->restart(1);
		}
	});
	this->addChild(restart_button, 1);
	//�������˵���ť
	auto back_button = Button::create("button.png");
	back_button->setScale(2);
	back_button->setTitleText(ReadJson::getString("returnMenu"));
	back_button->setTitleFontName("΢���ź�");
	back_button->setTitleFontSize(16);
	back_button->setPosition(Vec2(_screenWidth / 2, _screenHeight*0.4));
	back_button->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
		if (type == Widget::TouchEventType::ENDED){
			this->restart(2);
		}
	});
	this->addChild(back_button, 1);
}

//restart���������л�����
void my::Game::restart(int tag){
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	if (tag == 1){
		Director::getInstance()->replaceScene(my::Game::createScene());
	}
	else{
		Director::getInstance()->replaceScene(GameMenu::createScene());
	}
}


//startBoss��������������BOSS
void my::Game::startBoss(){
	//ɾ����Ļ���ел�
	for (unsigned int i = 0; i < _enemyVector.size(); i++){
		auto enemy = _enemyVector.at(i);
		if (enemy->getTag() == E1TAG){
			//���ú͵л������Ľ�����Ϊ0
			enemy->getHP()->setPercent(0);
		}
		this->removeChild(enemy, true);
	}
	//����л�����
	_enemyVector.clear();
	//������BOSS��������λ�á����ƺ�����ֵ
	_boss = FKSprite::createWithSpriteFrameName("e-10.png");
	_boss->setPosition(_screenWidth / 2, _screenHeight + _boss->getContentSize().height);
	_boss->setTag(BOSSTAG);
	_boss->setLifeValue(100);
	//������BossѪ��
	auto loadingBar = LoadingBar::create("planeHP.png");
	loadingBar->setScale(0.4);
	//���ý���������
	loadingBar->setDirection(LoadingBar::Direction::LEFT);
	//���ý�������ʼֵΪ100������Ѫ
	loadingBar->setPercent(100);
	//����Ϊ��Boss��Ѫ��
	_boss->setHP(loadingBar);
	this->addChild(_boss->getHP(), 1);
	_boss->setHPInterval(100 / _boss->getLifeValue());
	//��Boss������ӵ��л�����;����
	_enemyVector.pushBack(_boss);
	this->addChild(_boss, 4);
	//��Boss�ƶ�����Ļ�Ϸ����ƶ���ɺ����moveBoss����
	auto moveTo = MoveTo::create(2, Vec2(_screenWidth / 2, _screenHeight - _boss->getContentSize().height));
	auto action = Sequence::create(moveTo, CallFunc::create([=]{
		_isMove = true;
		moveBoss();
	}), NULL);
	//��BOSSִ�ж���
	_boss->runAction(action);

}

//moveBoss�������ڴ�Boss����������Ļ�Ϸ����������˶�
void my::Game::moveBoss(){
	if (_isMove){
		//_isShoot = true��ʾ��Boss���Կ�ʼ�����ڵ�
		isShoot = true;
		//��Boss��ͣ������Ļ�Ϸ�����������˶�
		Size bossContentSize = _boss->getContentSize();
		//����ƶ�����
		auto moveLeft = MoveTo::create(2.5, Vec2(_screenWidth - bossContentSize.width / 2, _screenHeight - bossContentSize.height));
		//�ұ��ƶ�����
		auto moveRight = MoveTo::create(2.5, Vec2(bossContentSize.width / 2, _screenHeight - bossContentSize.height));
		//�ӳٶ���
		auto delay = DelayTime::create(1);
		//��϶���
		auto sequence = Sequence::create(delay, moveLeft, moveRight, NULL);
		//�ظ�����
		auto action = RepeatForever::create(sequence);
		//��BOSSִ�ж���
		_boss->runAction(action);
	}
}

//shootBoss�������ڴ�BOSS��ʼ�����ڵ�
void my::Game::shootBoss(float delta){
	if (isShoot){
		if (_count % 80 == 0){
			for (int i = 6; i <= 180; i += 18){
				//��ô�BOSSλ��x��y������
				float startX = _boss->getPositionX();
				float startY = _boss->getPositionY();
				//������BOSS���ڵ�
				Sprite * bullet = Sprite::create("enemy_bullet1.png");
				//�����ڵ�����
				bullet->setPosition(startX, startY);
				//���ڵ�������ӵ���ǰ��
				this->addChild(bullet);
				/* ��r2Ϊ�뾶��Բ������Խǵ�x��y�����ӵ������λ��
				��ѧ�е����Ǻ�����cosf(���Һ���)��sinf(���Һ���)
				*/
				float r2 = _screenHeight + bullet->getContentSize().height / 2;
				float endX = startX - r2*cosf(i*M_PI / 180);
				float endY = startY - r2*sinf(i*M_PI / 180);
				//�ڵ��ƶ�����
				MoveTo *moveTo = MoveTo::create(3.0f, Vec2(endX, endY));
				//��ӵ���BOSS�ڵ�����
				_bossBulletVecor.pushBack(bullet);
				//�ڵ�ִ�ж���
				bullet->runAction(moveTo);
			}
		}
	}
}

//�������´�Boss��Ѫ��
void my::Game::updateHUD(float delta){
	//����Ѫ����λ�ã�Ѫ������boss�ƶ�
	if (_boss != NULL && _boss->getLifeValue() > 0){
		//boss�ľ�������
		Vec2 pos = _boss->getPosition();
		//BossѪ����ʾ����λ��
		_boss->getHP()->setPosition(Vec2(pos.x, pos.y+100));
	}
}