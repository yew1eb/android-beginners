/*
ProjectName: ����ս��
Author: ����С���
Blog: http://www.cnblogs.com/msxh/
Github:https://github.com/XINCGer
Date: 2016/04/01
*/
#include "PreLoadScene.h"
USING_NS_CC;

Scene* PreLoad::createScene(){
	// ����һ���������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
	auto scene = Scene::create();
	// ��������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
	auto layer = PreLoad::create();
	// ��PreLoad����Ϊ�ӽڵ���ӵ�����
	scene->addChild(layer);
	// ���س�������
	return scene;
}

bool PreLoad::init(){
	//���ø����init����
	if (!Layer::init()){
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//����һ��������
	auto barSprite = Sprite::create("loadingbar.png");
	//��ʼ��һ��ProgressTimer����������
	_progress = ProgressTimer::create(barSprite);
	//setPercentage:0.0f ,��ʾ��δ�����κ���Դ����ʾ����Ļ�Ͼ���ʲôҲ������
	_progress->setPercentage(0.0f);
	//����ͼƬ�Ĵ�С��ϵ����scale���ó�0.5������Сһ��
	//_progress->setScale(0.5f);
	//���ý�������������ʼλ�ã�Ĭ����ͼƬ���е�
	//�����Ҫ��ʾ�����ҵ�һ������Ч��������ĳ�(0,y)
	_progress->setMidpoint(Vec2(0.0f, 0.5f));
	//barChangeRate��ʾ�Ƿ�ı�ˮƽ���ߴ�ֱ����ı��������ó�1��ʾ�ı䣬0��ʾ���ı�
	_progress->setBarChangeRate(Vec2(1.0f, 0.0f));
	//ProgressTimerTypeBar��ʾΪ���ν�����
	_progress->setType(ProgressTimer::Type::BAR);
	//���ý�����λ������Ļ����
	_progress->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	//�����������Ϊ��ǰ����ӽڵ�
	this->addChild(_progress);
	return true;
}

void PreLoad::onEnterTransitionDidFinish(){
	//���ø����OnEnterTransitionDidFinish����
	Layer::onEnterTransitionDidFinish();
	//����preloadResources.plist�����ļ�����ȡ�ļ��е���Ϸ��Դ�����б�����һ��ValueMap����
	ValueMap map = FileUtils::getInstance()->getValueMapFromFile("preloadResources.plist");
	//ͨ��keyֵȡ��ÿ�ֲ�ͬ������Դ��ValueVector����
	ValueVector spriteSheets = map.at("SpriteSheets").asValueVector();
	ValueVector effects = map.at("Sounds").asValueVector();
	ValueVector musics = map.at("Musics").asValueVector();
	//���ValueVector�����size��ӵõ���Ҫ���ص���Դ������
	_sourceCount = spriteSheets.size() + effects.size() + musics.size();
	//���ý��������½���=100 /_sourceCount
	_progressInterval = 100 / _sourceCount;
	//���μ�����Դ
	loadMusic(musics);
	loadEffect(effects);
	loadSpriteSheets(spriteSheets);

}

//���ر�������
void PreLoad::loadMusic(ValueVector musicFiles){
	for (unsigned int i = 0; i != musicFiles.size(); ++i){
		Value v = musicFiles.at(i);
		SimpleAudioEngine::getInstance()->preloadBackgroundMusic(v.asString().c_str());
		progressUpdate();
	}

}

//��������
void PreLoad::loadEffect(ValueVector effectFiles){
	for (unsigned int i = 0; i != effectFiles.size(); ++i){
		Value v = effectFiles.at(i);
		SimpleAudioEngine::getInstance()->preloadEffect(v.asString().c_str());
		progressUpdate();
	}

}

//���ؾ����
void PreLoad::loadSpriteSheets(ValueVector spriteSheets){
	//�ú����������plist�ļ�������ͬ����׺��Ϊpng������ͼƬ
	//�Ѹ�plist������spriteFrame��Ϣ��ȡ��������֮��Ĵ����п���ͨ��
	//Sprite::createWithSpriteFrameName(const std::string& spriteFrameName)����
	//��ȡ��Ӧ�ľ���֡
	for (unsigned int i = 0; i != spriteSheets.size(); ++i){
		Value v = spriteSheets.at(i);
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(v.asString().c_str());
		progressUpdate();
	}


}

//���½���������
void PreLoad::progressUpdate(){
	//ÿ�ε��øú���˵������һ����Դ���Լ�������Դ����
	if (--_sourceCount){
		//���ý������ٷֱ�
		//_progress->setPercentage(100.0f - (_progressInterval*_sourceCount));
	}
	else{
		//ProgressFromTo�����Խ����ķ�ʽ��ʾͼƬ
		//��һ��������ʾ����0.5s,�ڶ���������ʾ�������İٷֱȿ�ʼ��������������ʾһֱ��100
		//auto pft = ProgressFromTo::create(0.5f, _progress->getPercentage(), 100);
		auto pft = ProgressFromTo::create(0.5f, 0, 100);
		//������һ��callFunc����
		auto callFunc = CallFunc::create([=]{
			//��������Դ��ɣ��ӳ�2����ת����Ϸ������
			auto delay = DelayTime::create(2.0f);
			auto callFunc = CallFunc::create([]{
				Director::getInstance()->replaceScene(my::Game::createScene());
			});
			auto action = Sequence::create(pft,callFunc,NULL);
			this->runAction(action);
		});
		auto action = Sequence::create(pft, callFunc, NULL);
		_progress->runAction(action);
	}

}

