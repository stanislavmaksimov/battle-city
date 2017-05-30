import QtQuick 2.7
import QtQuick.Window 2.2
import BattleCity 1.0

Window {
    title: qsTr("BattleCity")
    color: "#000000"
    visible: true

    width: engine.size.width
    height: engine.size.height
    minimumWidth: engine.size.width
    minimumHeight: engine.size.height

    onActiveChanged: if (!active) input.reset()
    Component.onCompleted: input.keysChanged.connect(engine.setKeys)

    Engine { id: engine }
    InputHandler { id: input }

    Item {
        focus: true
        Keys.onPressed: input.press(event.key, event.isAutoRepeat)
        Keys.onReleased: input.release(event.key, event.isAutoRepeat)

        anchors.centerIn: parent
        width: engine.size.width
        height: engine.size.height

        Repeater {
            model: engine.model
            delegate: tileComponent
        }

        Repeater {
            model: engine.model
            delegate: projectileComponent
        }
    }

    Component {
        id: tileComponent
        Tile {
            x: rect_role.x
            y: rect_role.y
            width: rect_role.width
            height: rect_role.height
            source: sprite_role
            mask: mask_role
            visible: visible_role
            invulnerableVisible: invulnerable_role
        }
    }

    Component {
        id: projectileComponent
        Rectangle {
            color: "lightgray"
            radius: 5
            width: radius * 2
            height: radius * 2
            x: projectile_position_role.x - width / 2
            y: projectile_position_role.y - height / 2
            visible: projectile_visible_role
        }
    }

    /*
    ListView {
        id: debugView
        opacity: 0.25
        anchors {
            top: parent.top
            left: parent.left
            bottom: parent.bottom
        }

        model: engine.model
        delegate: Text {
            color: visible_role ? "white" : "orange"
            text: index + " " + sprite_role
        }
    }
    */
}
