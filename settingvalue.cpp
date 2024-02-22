#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <matjson.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include <Geode/loader/SettingNode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

class MySettingValue : public SettingValue {
public:
    bool load(matjson::Value const& json) override {}
    bool save(matjson::Value& json) const override {}
    MySettingNode* createNode(float width) override {
        return MySettingNode::create();
    }
};
