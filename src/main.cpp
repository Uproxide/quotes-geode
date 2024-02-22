#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <matjson.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>

using namespace geode::prelude;

//auto data = Mod::get()->getSavedValue<json>("list");

void refreshFunc() {
	web::AsyncWebRequest()
    .fetch("http://projectbdash.com/api/v1/funfacts/fact/1.0.1.php")
    .then([this](auto const& json) {
			Mod::get()->setSavedValue<std::string>("list", json.dump(matjson::NO_INDENTATION););
    })
    .expect([this](std::string const& error) {
			// error :(
    });
}

class $modify(refresh) {
	void onPress(CCObject*) {
		refreshFunc();
	}
};



class MySettingValue : public SettingValue {
public:
    bool load(matjson::Value const& json) override {}
    bool save(matjson::Value& json) const override {}
    SettingNode* createNode(float width) override {
        return MySettingNode::create(width);
    }
};

class MySettingNode : public SettingNode {
protected:
    bool init(float width) {
        if (!SettingNode::init())
            return false;
        
        // You may change the height to anything, but make sure to call 
        // setContentSize!
        this->setContentSize({ width, 40.f });

        // Set up the UI. Note that Geode provides a background for the 
        // setting automatically
	auto spr = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");

	auto btn = CCMenuItemSpriteExtra::create(
		spr, this, menu_selector(refresh::onPress)
	);
	    
        return true;
    }

public:
    void commit() override {
        this->dispatchCommitted();
    }
    bool hasUncommittedChanges() override {
    }
    bool hasNonDefaultValue() override {
    }
    void resetToDefault() override {
    }

    static MySettingNode* create(MySettingValue* value, float width) {
        auto ret = new MySettingNode();
        if (ret && ret->init(value, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

$on_mod(Loaded) {
    Mod::get()->addCustomSetting<MySettingValue>("refresh-btn");
}

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
		auto data Mod::get()->getSavedValue<std::string>("list");
		if(auto object = matjson::parse(data)) {
				auto& firstObject = json[rand() % data.size()];
				std::string quote = fmt::format("{}\nBy {}", firstObject["funFact"].template as<std::string>(), firstObject["userOfReq"].template as<std::string>());
				FLAlertLayer::create(
					"Quote",
					quote,
					"OK"
				)->show();
            })
	}
};
