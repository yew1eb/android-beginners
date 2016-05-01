/*
ProjectName: ����ս��
Author: ����С���
Blog: http://www.cnblogs.com/msxh/
Github:https://github.com/XINCGer
Date: 2016/04/01
*/

#include "AudioControlScene.h"
#include "ui/CocosGUI.h"
#include "ReadJson.h"
USING_NS_CC;
using namespace ui;
using namespace cocos2d::extension;

bool AudioControl::init(){
	if (!Layer::init()){
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();

	/******************�������ֱ�ǩ�ͻ������*******************/
	//�������������֡��ı������Ϊ��ǰ����ӽڵ�
	auto music_text = Text::create(ReadJson::getString("bgMusic"), "Arial", 32);
	music_text->setFontName("΢���ź�");
	music_text->setPosition(Vec2(visibleSize.width*0.25,visibleSize.height*0.7));
	this->addChild(music_text);

	//����һ��������
	auto music_slider = Slider::create();
	//���û�����ԭ��
	music_slider->loadBarTexture("sliderTrack.png");
	//���û������Ĺ�������
	music_slider->loadSlidBallTextures("sliderThumb.png", "sliderThumb.png", "");
	//���û������Ľ���������
	music_slider->loadProgressBarTexture("sliderProgress.png");
	//��ȡ֮ǰ���õı�����������
	float musicPercent = UserDefault::getInstance()->getFloatForKey("musicPercent");
	//����ǵ�һ�ν������ó��������ñ������ֻ�����Ĭ��ֵΪ100
	if (musicPercent == 0.0f){
		musicPercent = 100.0f;
	}
	//���ñ������ֻ������ĳ�ʼֵ
	music_slider->setPercent(musicPercent);
	music_slider->setPosition(Vec2(visibleSize.width*0.6, visibleSize.height*0.7));
	//����¼�������������������������
	music_slider->addEventListener([=](Ref *pSender, Slider::EventType type){
		//���������İٷֱȷ����仯ʱ
		if (type == Slider::EventType::ON_PERCENTAGE_CHANGED){
			//��û������ٷֱ�
			int percent = music_slider->getPercent();
			//���ñ�������ֵΪ�������ٷֱ�/100,��ΪSlider�İٷֱ���1~100����MusicVolume��ȡֵ��0.0~1.0
			SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(float(percent) / 100);
			//�洢���õı�������ֵ
			UserDefault::getInstance()->setFloatForKey("musicPercent", percent);
		}
	});
	this->addChild(music_slider);

	/******************************��Ч������ǩ�ͻ��������*************************/
	//��������Ч�������ı������Ϊ��ǰ����ӽڵ�
	auto sound_text = Text::create(ReadJson::getString("effectMusic"), "Arial", 32);
	sound_text->setFontName("΢���ź�");
	sound_text->setPosition(Vec2(visibleSize.width*0.25, visibleSize.height*0.5));
	this->addChild(sound_text);
	//����һ��������
	auto effect_slider = Slider::create();
	//���û�����������
	effect_slider->loadBarTexture("sliderTrack.png");
	effect_slider->loadSlidBallTextures("sliderThumb.png", "sliderThumb.png", "");
	effect_slider->loadProgressBarTexture("sliderProgress.png");
	//��ȡ֮ǰ���õ���Ч����
	float effectPercent = UserDefault::getInstance()->getFloatForKey("effectPercent");
	//����ǵ�һ�ν������ó�����������Ч������Ĭ�ϳ�ʶֵΪ100
	if (effectPercent == 0.0f){
		effectPercent = 100.0f;
	}
	//������Ч�������ĳ�ʼֵ
	effect_slider->setPercent(effectPercent);
	effect_slider->setPosition(Vec2(visibleSize.width*0.6, visibleSize.height*0.5));
	//����¼���������������Ч����
	effect_slider->addEventListener([=](Ref *pSender, Slider::EventType type){
		//������İٷֱȷ����仯ʱ
		if (type == Slider::EventType::ON_PERCENTAGE_CHANGED){
			//��û����ٷֱ�
			int percent = effect_slider->getPercent();
			//���ñ�����Чֵ
			SimpleAudioEngine::getInstance()->setEffectsVolume(float(percent) / 100);
			//�洢���õı�������ֵ
			UserDefault::getInstance()->setFloatForKey("effectPercent", percent);
		}
	});
	this->addChild(effect_slider);

	//���������ء���ť�����ʱ����returnToMenu����
	auto return_button = Button::create("b1.png");
	return_button->setPosition(Vec2(visibleSize.width - return_button->getContentSize().width / 2,
		return_button->getContentSize().height / 2));
	return_button->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type){
		if (type == Widget::TouchEventType::ENDED){
			//�л���MenuScene����
			auto transtion = TransitionSlideInL::create(2.0, GameMenu::createScene());
			Director::getInstance()->replaceScene(transtion);
		}
	});
	this->addChild(return_button);

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

Scene* AudioControl::createScene(){
	// ����һ���������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
	auto scene = Scene::create();
	// ��������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
	auto layer = AudioControl::create();
	// ��GameSet����Ϊ�ӽڵ���ӵ�����
	scene->addChild(layer);
	// ���س�������
	return scene;
}