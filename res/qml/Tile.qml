import QtQuick 2.7

Item {
    height: TILE
    width: TILE
    smooth: false

    property string source: ""
    property string mask: ""
    property alias invulnerableVisible: invulnerable.visible

    Image {
        id: img
        anchors.fill: parent
        fillMode: Image.Tile
        horizontalAlignment: Qt.AlignLeft
        verticalAlignment: Qt.AlignTop
        visible: parent.source !== ""
        source: parent.source
    }

    Image {
        id: msk
        visible: mask !== "" && parent.source !== ""
        anchors.fill: parent
        smooth: false
        source: mask
        layer.enabled: mask !== "" && parent.source !== ""
        layer.effect: ShaderEffect {
            property variant innerSource: ShaderEffectSource {
                sourceItem: img
                hideSource: true
            }
            fragmentShader: "
                uniform lowp sampler2D source;
                uniform lowp sampler2D innerSource;
                uniform lowp float qt_Opacity;
                varying highp vec2 qt_TexCoord0;
                void main() {
                    lowp vec4 mskColor = texture2D(source, qt_TexCoord0.st);
                    lowp vec4 srcColor = texture2D(innerSource, qt_TexCoord0.st);
                    gl_FragColor = srcColor * mskColor.a * qt_Opacity;
                }"
        }
    }

    Invulnerable {
        id: invulnerable
        anchors.fill: parent
    }
}
