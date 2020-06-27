import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

Rectangle {
  id: window
  width: 1280
  height: 720
  focus: true
  color: "black"

  GridView {
    id: id_gridView
    anchors.fill: parent

    cellWidth: width/4
    cellHeight: height/2

    model: channelModel
    delegate: Item {
      // Main rectangle showing the outline of the cell
      Rectangle {
        id: id_mainRect
        width: id_gridView.cellWidth - 5
        height: id_gridView.cellHeight - 5
        color: "#2b2d2f"
      }

      // Channel name
      Text {
        id: id_channelName
        anchors.top: id_mainRect.top
        anchors.topMargin: 5
        color: "white"
        text: "Channel " +  model.modelData.id;
        font.pointSize: 20
        anchors.left: id_mainRect.left
        anchors.leftMargin: 5
      }

      // on/off circle
      Rectangle {
        id: id_offCircle
        anchors.top: id_mainRect.top
        anchors.right: id_mainRect.right
        anchors.rightMargin: 5
        anchors.topMargin: 10
        width: 20
        height: 20
        color: model.modelData.state ? "green" : "red"
        border.color: "black"
        border.width: 1
        radius: width*0.5
      }

      // Separator
      Rectangle {
        id: id_separator
        height: 2
        width: id_gridView.cellWidth
        color: "black"
        anchors.top: id_channelName.bottom
        anchors.topMargin: 2
      }

      // Master switch label
      Text {
        id: id_masterLabel
        text: "Master";
        color: "white"
        anchors.top: id_separator.bottom
        anchors.topMargin: 2
        font.pointSize: 14
        anchors.horizontalCenter: id_mainRect.horizontalCenter
        horizontalAlignment: Text.AlignHCenter
      }

      // Actual master switch
      Switch {
        id: id_masterSwitch
        anchors.top: id_masterLabel.bottom
        anchors.horizontalCenter: id_masterLabel.horizontalCenter
        checked: model.modelData.master

        onToggled: {
          adapter.onMasterSwitched(model.modelData.id, checked)
        }

        indicator: Rectangle {
          implicitWidth: 48
          implicitHeight: 26
          x: id_masterSwitch.leftPadding
          y: parent.height / 2 - height / 2
          radius: 13
          color: id_masterSwitch.checked ? "green" : "red"
          border.color: id_masterSwitch.checked ? "green" : "red"

          Rectangle {
            x: id_masterSwitch.checked ? parent.width - width : 0
            width: 26
            height: 26
            radius: 13
            color: id_masterSwitch.down ? "#cccccc" : "#ffffff"
            border.color: id_masterSwitch.checked ? (id_masterSwitch.down ? "#17a81a" : "#21be2b") : "#999999"
          }
        }
      }

      // Override label
      Text {
        id: id_overrideLabel
        text: "Override";
        color: "white"
        anchors.top: id_masterSwitch.bottom
        anchors.topMargin: 5
        font.pointSize: 14
        anchors.horizontalCenter: id_mainRect.horizontalCenter
        horizontalAlignment: Text.AlignHCenter
      }

      // Actual override switch
      Switch {
        id: id_overrideSwitch
        anchors.top: id_overrideLabel.bottom
        anchors.horizontalCenter: id_overrideLabel.horizontalCenter
        checked: model.modelData.overridden && model.modelData.master
        enabled: model.modelData.master

        onToggled: {
          adapter.onOverriddenSwitched(model.modelData.id, checked)
        }

        indicator: Rectangle {
          implicitWidth: 48
          implicitHeight: 26
          x: id_overrideSwitch.leftPadding
          y: parent.height / 2 - height / 2
          radius: 13
          color: id_overrideSwitch.enabled ? (id_overrideSwitch.checked ? "green" : "red") : "#444444"
          border.color: id_overrideSwitch.enabled ? (id_overrideSwitch.checked ? "green" : "red") : "#444444"

          Rectangle {
            x: id_overrideSwitch.checked ? parent.width - width : 0
            width: 26
            height: 26
            radius: 13
            color: id_overrideSwitch.enabled ? (id_overrideSwitch.down ? "#cccccc" : "#ffffff") : "#777777"
            border.color: id_overrideSwitch.checked ? (id_overrideSwitch.down ? "#17a81a" : "#21be2b") : "#999999"
          }
        }
      }

      // Testing label of next on time
      Text {
        id: id_nextLabel
        text: model.modelData.testNextOn
        color: "white"
        font.pointSize: 14

        anchors.top: id_overrideSwitch.bottom
        anchors.horizontalCenter: id_overrideSwitch.horizontalCenter
        anchors.topMargin: 5
      }

      // Testing, day
      ComboBox {
        id: id_testDayCombobox

        anchors.top: id_nextLabel.bottom
        anchors.horizontalCenter: id_nextLabel.horizontalCenter
        anchors.topMargin: 5

        onActivated: {
          console.log("Current value: " + currentText)
        }

        model: ["Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"]
      }

      // Testing, hour
      ComboBox {
        id: id_testHourCombobox

        anchors.top: id_testDayCombobox.bottom
        anchors.horizontalCenter: id_nextLabel.horizontalCenter
        anchors.topMargin: 5

        onActivated: {
          console.log("Current value: " + currentText)
        }

        model: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23]
      }

      // Testing, minute
      ComboBox {
        id: id_testMinuteCombobox

        anchors.top: id_testHourCombobox.bottom
        anchors.horizontalCenter: id_nextLabel.horizontalCenter
        anchors.topMargin: 5

        onActivated: {
          console.log("Current value: " + currentText)
        }

        model: [0, 15, 30, 45]
      }

      // Testing, accept button
      Button {
        id: id_testAcceptButton
        text: "Accept"

        onPressed: {
          adapter.onTimerAccept(model.modelData.id, id_testDayCombobox.currentText, id_testHourCombobox.currentText, id_testMinuteCombobox.currentText)
        }

        anchors.top: id_testMinuteCombobox.bottom
        anchors.horizontalCenter: id_nextLabel.horizontalCenter
        anchors.topMargin: 5
      }
    }
  }
}
