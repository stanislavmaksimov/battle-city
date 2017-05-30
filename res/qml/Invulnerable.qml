import QtQuick 2.7

SpriteSequence {
    smooth: false
    running: true
    sprites: [
        Sprite {
            name: "invulnerable0"
            source: SPRITE_NAME_INVULNERABLE +"/0"
            frameCount: 1
            frameDuration: 50
            frameDurationVariation: 25
            to: { "invulnerable1" : 1 }
        },
        Sprite {
            name: "invulnerable1"
            source: SPRITE_NAME_INVULNERABLE +"/1"
            frameCount: 1
            frameDuration: 50
            frameDurationVariation: 25
            to: { "invulnerable0" : 1 }
        }
    ]
}
