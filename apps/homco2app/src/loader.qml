import QtQuick 2.12

Loader {
  id: id_mainLoader
  source: "main.qml"

  Shortcut {
    sequence: "Q"
    context: Qt.ApplicationShortcut
    onActivated: {
      console.log("shortcut")
      id_mainLoader.source = ""
      id_mainLoader.active = false
      app.reloadQml()
      id_mainLoader.active = true
      id_mainLoader.source = "main.qml"
    }
  }
}