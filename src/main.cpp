#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <matjson.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>

using namespace geode::prelude;

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

		menu->addChild(btn);
		menu->updateLayout();
		return true;
	} 

	void onFunFact(CCObject*) {
		web::AsyncWebRequest()
            .fetch("http://projectbdash.com/api/v1/funfacts/fact/")
            .json()
            .then([this](auto const& json) {
                log::info("{}", json);
				auto& firstObject = json[0];
				log::info("{}", firstObject["funFact"]);
				log::info("{}", firstObject["userOfReq"]);
				std::string quote = fmt::format("{}\nBy {}", firstObject["funFact"].template as<std::string>(), firstObject["userOfReq"].template as<std::string>());
				FLAlertLayer::create(
					"Quote",
					quote,
					"OK"
				)->show();
            })
            .expect([this](std::string const& error) {
				// error :(
            });
	}
};
