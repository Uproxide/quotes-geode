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
	   .fetch("http://projectbdash.com/api/v1/funfacts/fact/v1.0.1.php")
	   .text()
	   .then([&](auto const& webRes){
		   Mod::get()->setSavedValue<std::string>("list", webRes.c_str()); 
	   });
        //auto webRes = webReq.text();
	//Mod::get()->setSavedValue<std::string>("list", webRes.c_str());
}


// Stole from gd utils because stuff on the docs wouldent work so yea
SettingNode* SettingRefreshValue::createNode(float width) {
    return SettingRefreshNode::create(this, width);
}

void SettingRefreshNode::onRefreshBtn(CCObject*) {
	    refreshFunc();
}



$on_mod(Loaded) {
    Mod::get()->addCustomSetting<SettingRefreshValue>("refresh-btn", "none");
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
		auto json2 = matjson::parse(fmt::format("{\"thereeldata\":{}}", data));
		auto json2 = json2["thereeldata"]
		//auto resAsVector = json.as<std::array>();
				auto& firstObject = json[rand() % json.size()];
				std::string quote = fmt::format("{}\nBy {}", firstObject["funFact"], firstObject["userOfReq"]);
				FLAlertLayer::create(
					"Quote",
					quote,
					"OK"
				)->show();
	}
};
