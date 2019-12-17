import QtQuick 2.0

Rectangle {
    id: window
    width: 480
    height: 480
    focus: true
    color: "#272822"

    Connections {
      target: adapter
      onCreated: {
        console.log("Created!");
        adapter.onTest();
      }
    }
}