#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <matjson.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include <Geode/loader/SettingNode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
// Thanks so much Jouca and Firee
#include "settings.hpp"

using namespace geode::prelude;

//auto data = Mod::get()->getSavedValue<json>("list");

void refreshFunc() {
	web::AsyncWebRequest()
    .fetch("http://projectbdash.com/api/v1/funfacts/fact/1.0.1.php")
    .json()
    .then([this](auto const& json) {
		//matjson::Value json = this
		Mod::get()->setSavedValue<std::string>("list", json.dump(matjson::NO_INDENTATION));
    })
    .expect((std::string error) {});
}


// Stole from gd utils because stuff on the docs wouldent work so yea
SettingNode* SettingRefreshValue::createNode(float width) {
    return SettingRefreshNode::create(this, width);
}

void SettingRefreshNode:onRefreshBtn(CCObject*) {
	    refreshFunc();
}



$on_mod(Loaded) {
    Mod::get()->addCustomSetting<SettingTestValue>("refresh-btn", "none");
}
// end of stealing from GDUtils THANKS!


class $modify(funFacts, MenuLayer) {
	std::string funFact;
	
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		} 
		auto menu = static_cast<cocos2d::CCMenu*>(this->getChildByID("bottom-menu"));
		auto spr = CCSprite::createWithSpriteFrameName("GJ_chatBtn_001.png");

		auto btn = CCMenuItemSpriteExtra::create(
			spr, this, menu_selector(funFacts::onFunFact)
		);


		if (!Mod::get()->setSavedValue("notFirstLaunch", true)) {
    			refreshFunc();
		}
		
		menu->addChild(btn);
		menu->updateLayout();
		return true;
	} 

	void onFunFact(CCObject*) {
		auto data = Mod::get()->getSavedValue<std::string>("list");
		auto json = matjson::parse(data);
				auto& firstObject = json[rand() % data.size()];
				std::string quote = fmt::format("{}\nBy {}", firstObject["funFact"], firstObject["userOfReq"]);
				FLAlertLayer::create(
					"Quote",
					quote,
					"OK"
				)->show();
	}
};
