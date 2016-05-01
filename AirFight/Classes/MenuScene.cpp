/*
ProjectName: ����ս��
Author: ����С���
Blog: http://www.cnblogs.com/msxh/
Github:https://github.com/XINCGer
Date: 2016/04/01
*/
#include "MenuScene.h"
#include "ui\CocosGUI.h"
#include "PreLoadScene.h"
#include "ReadJson.h"

using namespace cocos2d;
using namespace ui;

bool GameMenu::init(){
	//���ø����init����
	if (!Layer::init()){
		return false;
	}
	//����豸�ɼ���ͼ��С
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//��ʼ��Ϸ��ť
	auto start_button = Button::create("button.png");
	start_button->setScale(2);
	start_button->setTitleText(ReadJson::getString("start"));
	start_button->setTitleFontName("΢���ź�");
	start_button->setTitleFontSize(16);
	start_button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*0.7));
	start_button->addTouchEventListener([](Ref *pSender, Widget::TouchEventType type){
		if (type == Widget::TouchEventType::ENDED){
			//�л���PreLoad����
			auto transition = TransitionSlideInL::create(2.0, PreLoad::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});

	this->addChild(start_button);

	//��Ϸ���ð�ť
	auto set_button = Button::create("button.png");
	set_button->setScale(2);
	set_button->setTitleText(ReadJson::getString("setting"));
	set_button->setTitleFontName("΢���ź�");
	set_button->setTitleFontSize(16);
	set_button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*0.55));
	set_button->addTouchEventListener([](Ref *pSender, Widget::TouchEventType type){
		if (type == Widget::TouchEventType::ENDED){
			//�л���AudioControl����
			auto transtion = TransitionSlideInL::create(2.0, AudioControl::createScene());
			Director::getInstance()->replaceScene(transtion);
		}
	});
	this->addChild(set_button);

	//�˳���Ϸ��ť
	auto close_button = Button::create("button.png");
	close_button->setScale(2);
	close_button->setTitleText(ReadJson::getString("exitGame"));
	close_button->setTitleFontName("΢���ź�");
	close_button->setTitleFontSize(16);
	close_button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*0.4));
	close_button->addTouchEventListener([](Ref *pSender, Widget::TouchEventType type){
		if (type == Widget::TouchEventType::ENDED){
			Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
			MessageBox("�����˳���Ϸ��", "��ʾ��Ϣ");
			exit(0);
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			exit(0);
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			exit(0);
#endif
		}
	});
	this->addChild(close_button);
	//������
	auto banner_text = Text::create(ReadJson::getString("info"), "΢���ź�", 32);
	banner_text->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*0.15));
	this->addChild(banner_text);
	//��ַ
	auto blog_text = Text::create("www.cnblogs.com/msxh", "΢���ź�", 32);
	blog_text->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*0.1));
	this->addChild(blog_text);
	return true;
}

Scene* GameMenu::createScene()
{
	// ����һ���������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
	auto scene = Scene::create();
	// ��������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
	auto layer = GameMenu::create();
	// ��GameMenu����Ϊ�ӽڵ���ӵ�����
	scene->addChild(layer);
	// ���س�������
	return scene;
}