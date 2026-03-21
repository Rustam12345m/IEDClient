# Plan: Replace DataSet TextField with ComboBox in RCB Properties Panel

## Context
The RCB properties panel has a plain TextField for DataSet reference. Users must type the full dataset reference manually (e.g., `IEDDigitalTwinControl/LLN0$dsCtlSt`), which is error-prone. Replace it with a ComboBox populated from all available datasets in the IED model. If the current RCB's DataSet value isn't in the list (stale/unknown reference), show it as the first item highlighted in red.

## Implementation

### 1. Expose dataset list from backend
**File:** `src/app/ied_backend.hpp` + `.cpp`
- Add `Q_INVOKABLE QStringList getDataSetRefs() const`
- Iterates `m_con.m_ied->model().getDataSetList()`, builds full references: `ds->ref() + "$" + ds->getName()` for each dataset
- Returns sorted list

### 2. Replace TextField with ComboBox in RCB panel
**File:** `ui/reports/RCB_PropertiesPanel.qml`

Current:
```qml
TextField {
    Layout.fillWidth: true
    id: dataSetID
    text: ""
}
```

Replace with:
```qml
ComboBox {
    Layout.fillWidth: true
    id: dataSetID
    editable: false
    model: []

    // Custom delegate to show unknown values in red
    delegate: ItemDelegate {
        text: modelData
        contentItem: Text {
            text: modelData
            color: index === 0 && dataSetID.unknownValue ? "red" : palette.text
        }
    }

    property bool unknownValue: false
}
```

### 3. Update loadFromModel()
In `loadFromModel()`:
- Get the dataset list from backend: `iedBackend.getDataSetRefs()`
- Get the current RCB's dataset: `rcbModel.selectedDsRef()`
- If the current value is in the list, set `currentIndex` to it
- If NOT in the list, prepend it to the model and mark `unknownValue = true`, show it in red
- If the current value is empty, just show the list with no selection

### 4. Update sigEnable signal
The `onClicked` for Enable currently reads `dataSetID.text`. With a ComboBox, change to `dataSetID.currentText`.

### 5. Files to modify
- `src/app/ied_backend.hpp` + `.cpp` — add `getDataSetRefs()`
- `ui/reports/RCB_PropertiesPanel.qml` — replace TextField with ComboBox, update loadFromModel and Enable handler

### Verification
- Open RCB properties panel
- ComboBox shows all available datasets
- Selecting a different dataset and clicking Enable changes the RCB's dataset
- If the RCB has an unknown dataset reference, it appears first in red
- Empty dataset shows the list with no selection
