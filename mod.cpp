#include <Geode/Geode.hpp>
#include <Geode/ui/ModSettings.hpp>
#include <Geode/modify/CCTextureCache.hpp>
#include <cocos2d.h>
#include <map>

class MyMod : public geode::Mod {
public:
    std::map<std::string, std::string> originalTextures;

    virtual void onEnable() override {
        geode::log::info("Texture Randomizer Mod Enabled!");
    }

    virtual void onDisable() override {
        geode::log::info("Texture Randomizer Mod Disabled!");
    }

    void setupSettings() {
        auto settings = geode::ModSettings::create();
        settings->addButton("Randomize Textures", [this]() {
            randomizeTextures();
        });
        settings->addButton("Reset Textures", [this]() {
            resetTextures();
        });
        this->addSettingsPage(settings);
    }

    void randomizeTextures() {
        geode::log::info("Randomizing textures...");
        randomizeAllTextures();
    }

    void resetTextures() {
        geode::log::info("Resetting textures...");
        resetAllTextures();
    }

    void randomizeAllTextures() {
        originalTextures["texture1.png"] = "original_texture1.png";
        originalTextures["texture2.png"] = "original_texture2.png";
        originalTextures["texture3.png"] = "original_texture3.png";
    }

    void resetAllTextures() {
        for (const auto& entry : originalTextures) {
            const std::string& texture = entry.first;
            const std::string& original = entry.second;
            geode::log::info("Restoring: {} to {}", texture, original);
            CCTextureCache::sharedTextureCache()->addImage(original.c_str());
        }
    }
};

$register_mod(MyMod);

class $modify(CCTextureCache) {
    cocos2d::CCTexture2D* addImage(const char* fileImage) {
        if (isRandomizingTextures()) {
            std::string randomizedTexture = getRandomTexture(fileImage);
            return CCTextureCache::sharedTextureCache()->addImage(randomizedTexture.c_str());
        } else {
            return CCTextureCache::sharedTextureCache()->addImage(fileImage);
        }
    }

    bool isRandomizingTextures() {
        return true;
    }

    std::string getRandomTexture(const std::string& originalTexture) {
        std::vector<std::string> textures = {
            "random_texture1.png",
            "random_texture2.png",
            "random_texture3.png"
        };
        int randomIndex = rand() % textures.size();
        return textures[randomIndex];
    }
};
