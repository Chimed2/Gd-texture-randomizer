#include <Geode/Geode.hpp>
#include <Geode/ui/ModSettings.hpp>
#include <Geode/modify/CCTextureCache.hpp>
#include <cocos2d.h>
#include <map>
#include <vector>
#include <string>
#include <cstdlib>

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
        
        // Responsive buttons for both desktop and mobile
        settings->addButton("Randomize Textures", [this]() {
            randomizeTextures();
        }, "Randomizes all game textures.");
        
        settings->addButton("Reset Textures", [this]() {
            resetTextures();
        }, "Resets all textures to default.");
        
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
        // Store original textures to allow resetting later
        originalTextures["texture1.png"] = "original_texture1.png";
        originalTextures["texture2.png"] = "original_texture2.png";
        originalTextures["texture3.png"] = "original_texture3.png";

        // You can expand the list to include more textures for randomization
    }

    void resetAllTextures() {
        // Restore all original textures
        for (const auto& entry : originalTextures) {
            const std::string& texture = entry.first;
            const std::string& original = entry.second;
            geode::log::info("Restoring: {} to {}", texture, original);
            CCTextureCache::sharedTextureCache()->addImage(original.c_str());
        }
    }
};

$register_mod(MyMod);

// Modify the texture cache to randomize textures when requested
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
        // You can add conditions to enable/disable randomization here
        return true;
    }

    std::string getRandomTexture(const std::string& originalTexture) {
        // List of random textures to be used in place of the originals
        std::vector<std::string> textures = {
            "random_texture1.png",
            "random_texture2.png",
            "random_texture3.png"
        };
        int randomIndex = rand() % textures.size();
        return textures[randomIndex];
    }
};

#ifdef GEODE_IS_MOBILE
// Mobile-specific optimization or adjustments can go here
void optimizeForMobile() {
    // You could reduce the number of textures or optimize performance further
}
#endif
