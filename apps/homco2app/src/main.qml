import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

Rectangle {
  id: window
  width: 1280
  height: 720
  focus: true
  color: "#272822"

  GridLayout {
    anchors.fill: parent
    id: grid
    columns: 3

    Button {
      text: "Override ch 0"
      onClicked: console.log("clicked!")
    }
    Text {
      id: id_masterText0
      text: "master"
      color: "green"
    }
    Text {
      id: id_overriddenText0
      text: "overridden"
      color: "red"
    }
  }

  Connections {
    target: adapter
    onChannelUpdated: {
      // id, master, overridden
      if (id == 0) {
        if (master) {
          id_masterText0.color = "green";
        }
        else {
          id_masterText0.color = "red";
        }
        if (overridden) {
          id_overriddenText0.color = "green";
        }
        else {
          id_overriddenText0.color = "red";
        }
      }
    }
  }
}