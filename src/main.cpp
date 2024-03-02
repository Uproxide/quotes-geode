#include <Geode/Geode.hpp>
#include <Geode/utils/web2.hpp>
#include <matjson.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include <Geode/loader/SettingNode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
// Thanks so much Jouca and Firee
#include "settings.hpp"
#include <algorithm>
#include <string>

using namespace geode::prelude;

//auto data = Mod::get()->getSavedValue<json>("list");

auto launched = false;

void refreshFunc() {
	web::WebRequest()
	   .get("http://projectbdash.com/api/v1/funfacts/fact/v1.0.1.php")
	   .then((auto const& webRes){
		   Mod::get()->setSavedValue<std::string>("list", webRes.string().c_str()); 
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


		if (!launched) {
    			refreshFunc();launched = true;
		} else log::debug("Quote Array: {}", Mod::get()->getSavedValue<std::string>("list"));
		
		menu->addChild(btn);
		menu->updateLayout();
		return true;
	} 

	void onFunFact(CCObject*) {
		auto data = Mod::get()->getSavedValue<std::string>("list");
		//log::debug("Quote Array: {}", data);
		auto json2 = matjson::parse(fmt::format("{{\"thereeldata\":{} }}", data));
		auto json = json2["thereeldata"];
		auto jsonsize = json.as_array().size();
		auto& firstObject = json[rand() % jsonsize];
		if(Mod::get()->getSettingValue<bool>("questionable") == false){
			auto testy = json2["thereeldata"];
			auto testyasarr = testy.as_array();
			std::string testystring("{\"thereeldata\": [");
			auto testycount = 0;
			for (int i = 0; i < testyasarr.size();i++) {
				if (testy[i]["questionableQuote"] == 0){testystring=fmt::format("{1}{0},",i,testystring);testycount++;}
			}
			testystring = testystring.substr(0,testystring.size()-1);
			testystring = fmt::format("{}]}}",testystring);
			log::debug("The testytesty: {}",testystring);
			auto eeee = matjson::parse(testystring);
			firstObject = json[eeee["thereeldata"][rand() % testycount].as_int()];
		}
		std::string quote = fmt::format("{}\nBy {}", firstObject["funFact"], firstObject["userOfReq"]);
		FLAlertLayer::create(
			"Quote",
			quote,
			"OK"
		)->show();
	}
};
