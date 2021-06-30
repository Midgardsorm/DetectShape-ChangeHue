import QtQuick
import QtQuick.Controls
import Qt.labs.platform as Labs
import QtQuick.Layouts

Window {
  width: mainColumn.width
  height: mainColumn.height
  minimumWidth: mainColumn.width
  minimumHeight: mainColumn.height
  maximumWidth: mainColumn.width
  visible: true
  title: qsTr("Hue controller")
  flags: Qt.Dialog

  Column {
    id: mainColumn
    padding: 10
    spacing: 6

    Button {
      text: "Open file"
      onClicked: openDialog.open()
      width: 200
    }
    Rectangle {
      implicitWidth: 200
      implicitHeight: 1
      color: "black"
    }
    Label {
      text: "Triangles: " + HueController.detectedTriangles
    }
    Label {
      text: "Rectangles: " + HueController.detectedRectangles
    }
    Label {
      text: "Circles: " + HueController.detectedCircles
    }
    Label {
      text: "Elipses (non circles): " + HueController.detectedElipses
    }
    Button {
      text: "Detect shapes"
      enabled: HueController.isFileLoaded
      onClicked: HueController.detectShapes()
      width: 200
    }
    Rectangle {
      implicitWidth: 200
      implicitHeight: 1
      color: "black"
    }
    Rectangle {
      width: 200
      height:200
      ListView {
        id: shapeList
        anchors.fill: parent
        width: 200;
        implicitHeight: 200
        clip: true
        currentIndex: HueController.activeIndex
        onCurrentIndexChanged: hueSlider.value = 0

        highlight: Rectangle {
          color: "lightgrey"
        }

        ScrollBar.vertical: ScrollBar {
          policy: ScrollBar.AsNeeded
        }
        model: HueController.detectedShapes.length
        delegate: Text {
          text: HueController.detectedShapes[index]
          Layout.fillWidth: true
          MouseArea {
            anchors.fill: parent
            onClicked: {
              if (index != shapeList.currentIndex) {
                HueController.setActiveItemIndex(index)
              }
            }
          }
        }
      }
    }
    Rectangle {
      implicitWidth: 200
      implicitHeight: 1
      color: "black"
    }
    Label {
      text: "Hue:"
    }
    Slider {
      id: hueSlider
      enabled: shapeList.currentIndex !== -1
      width: 200
      from: 0
      value: 0
      to: 179
      onValueChanged: HueController.changeHue(value)
      onPressedChanged: if (!pressed) HueController.setMainImageWithCurrentHue()
    }
    Rectangle {
      implicitWidth: 200
      implicitHeight: 1
      color: "black"
    }
    Label {
      text: "Selection color (click below):"
    }
    Rectangle {
      height : 20
      width: 200
      border.color: "black"
      border.width: 1
      color: colorDialog.currentColor
      MouseArea {
        anchors.fill: parent
        onClicked: colorDialog.open()
      }
    }
  }

  Labs.ColorDialog {
    id: colorDialog
    currentColor: "blue"
    onAccepted: HueController.setSelectionColor(currentColor)
  }

  Labs.FileDialog {
    id: openDialog
    title: "Open file"
    fileMode: Labs.FileDialog.OpenFile
    nameFilters: ["PNG image files (*.png)"]
    onAccepted: {
      hueSlider.value = 0
      HueController.openFile(openDialog.file)
    }
  }
}
