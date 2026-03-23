# Known Issues & Suspicious Code

Potential crashes, memory leaks, and logic bugs found by code audit.

## Crashes (Null Pointer Dereference)

### 1. `LN_OverviewTable::data()` — null `m_ldev` dereference
**File:** `src/models/ln_overview_table.cpp:90`
```cpp
auto ln = m_ldev->getItem<Core::LogicalNode>(index.row());
```
`m_ldev` is dereferenced without null check. It can be null after `setActiveIED()` resets it, or before any LD is selected. `rowCount()` correctly guards with `if (m_ldev)`, but `data()` does not — Qt may call `data()` during model reset transitions.

**Fix:** Add `if (!m_ldev) return QVariant();` at the top of `data()`.

---

### 2. `HistConTable::data()` — no bounds check on `m_con` access
**File:** `src/models/history_con_table.cpp:64-74`
```cpp
case ColumnType::IED_NAME: {
    return m_con[index.row()].ied();  // no bounds check
}
```
Direct `m_con[index.row()]` access without verifying `index.row() < m_con.size()`. The list can be modified by `slotAppConfigUpdated()` (which does `beginResetModel` + replace) while Qt is still calling `data()` for old indices.

**Fix:** Add `if (index.row() < 0 || index.row() >= m_con.size()) return QVariant();` guard.

---

### 3. `MmsValue_toString()` may return NULL — `strncpy` crash
**File:** `src/libiec61850_stub/ied_state_api_impl.cpp:130`
```cpp
strncpy(tmp, MmsValue_toString(mmsValue), 256);
```
`MmsValue_toString()` can return NULL for certain MMS types. `strncpy(dst, NULL, n)` is undefined behavior — segfault on most platforms. Same pattern at line 256.

**Fix:** `const char *s = MmsValue_toString(mmsValue); if (s) strncpy(tmp, s, 255);`

---

### 4. `fetchLN_DS()` — null `dsList` dereference
**File:** `src/libiec61850_stub/ied_model_api_impl.cpp:187`
```cpp
LinkedList dsList = IedConnection_getLogicalNodeDirectory(...);
LinkedList dataSet = LinkedList_getNext(dsList);  // no null check on dsList
```
If the call fails, `dsList` is NULL. `LinkedList_getNext(NULL)` is a segfault. Compare with `fetchLN_DO()` at line 145 which correctly checks `(retval == IED_ERROR_OK) && (doList != nullptr)` before proceeding.

Same issue at line 199: `LinkedList_getNext(dsEntityList)` without null check.

**Fix:** Guard with `if (retval != IED_ERROR_OK || dsList == nullptr) return 0;`

---

### 5. `getValuesForDataObject()` — null `daNode` dereference
**File:** `src/libiec61850_stub/ied_state_api_impl.cpp:175-176`
```cpp
auto daNode = dataObj->getItem<Core::DataAttribute>(j);
auto fcNum = (FunctionalConstraint)daNode->fcNum();  // no null check
```
`getItem()` can return null if the child isn't a `DataAttribute` (e.g., it's a `DataObject`). The next line dereferences it unconditionally.

**Fix:** Add `if (!daNode) continue;`

---

### 6. `IEDConContainer::isConnected()` — null `m_api`
**File:** `src/app/iedcon_container.hpp:44-46`
```cpp
bool isConnected() const {
    return m_api->isConnected();
}
```
`createNewConnection()` calls `m_api.clear()` then reassigns. During that window `m_api` is null. `isConnected()` is called from QML property bindings and backend methods which could evaluate at any time.

**Fix:** `return m_api && m_api->isConnected();`

---

### 7. `ldsPageStatus()` — null `m_ied` dereference
**File:** `src/app/ied_backend.cpp:480`
```cpp
return "IED: " + m_con.m_ied->model().getName();
```
Called from QML via status bar binding. If called before connection or after disconnect, `m_ied` may point to an empty model but the access pattern assumes a populated model. In practice `m_ied` is always allocated (never truly null due to `createNewConnection`), but if the call chain changes this could crash.

**Risk:** Low — defensive check still recommended.

---

## Memory Leaks

### 8. `getFileList()` — LinkedList leaked on error
**File:** `src/libiec61850_stub/ied_fs_api_impl.cpp:73-76`
```cpp
LinkedList dirRoot = IedConnection_getFileDirectory(m_api.m_libConn, &retval, path.c_str());
if (retval != IED_ERROR_OK) {
    return -1;  // dirRoot may be non-null but is never freed
}
```
libiec61850 can return a partially-constructed list even on error. The early return skips the `LinkedList_destroyDeep` at line 94.

**Fix:** Add `if (dirRoot) LinkedList_destroyDeep(dirRoot, ...);` before the early return.

---

### 9. `fetchLN_DO()` — `doList` leaked when retval != OK but list allocated
**File:** `src/libiec61850_stub/ied_model_api_impl.cpp:176`
```cpp
LinkedList_destroy(doList);
```
This runs unconditionally after the `if` block at line 145. If `doList` is null (retval failed), `LinkedList_destroy(NULL)` is called. libiec61850 handles this gracefully (no-op on NULL), so no crash, but the pattern is inconsistent with the guarded access above.

**Risk:** Not a crash but confusing code.

---

## Logic Bugs

### 10. `getStatusForAllLN()` — unused loop variable in inner loop
**File:** `src/libiec61850_stub/ied_state_api_impl.cpp:216-220`
```cpp
for (size_t i=0; i<ld->getItemCount(); i++) {       // outer: iterates different LNs — correct
    auto ln = ld->getItem<Core::LogicalNode>(i);
    for (size_t j=0; j<ln->getItemCount(); j++) {    // inner: j is never used in body
        getValuesForDataObject(ln->getModItem(), vals, m_api.m_libConn);
        getValuesForDataObject(ln->getBehItem(), vals, m_api.m_libConn);
        getValuesForDataObject(ln->getHealthItem(), vals, m_api.m_libConn);
    }
}
```
The outer loop correctly iterates different LNs. However, the inner `for j` loop repeats the same three reads (Mod, Beh, Health) for each DataObject in the LN — `j` is never referenced in the body. For an LN with 20 DOs this makes 60 network reads instead of 3. Not a crash, but a performance issue that slows down LN status refresh.

**Fix:** Remove the inner `for` loop — read Mod/Beh/Health once per LN.

---

### 11. `fetchLN_DS()` — `dsList` destroyed even when null
**File:** `src/libiec61850_stub/ied_model_api_impl.cpp:212`
```cpp
LinkedList_destroy(dsList);
```
If `IedConnection_getLogicalNodeDirectory` fails and returns NULL, `LinkedList_destroy(NULL)` is called. Not a crash (libiec61850 handles it), but combined with issue #4, the whole function lacks error guards that the other `fetchLN_*` methods have.

---

## Race Conditions / Use-After-Free Risks

### 12. Report callback — use-after-free on `ReportStorage`
**File:** `src/libiec61850_stub/ied_control_api_impl.cpp:283-289`
```cpp
void IED_ControlAPI_Impl::staticReportCallback(void *param, void *report)
{
    auto *storage = static_cast<Core::ReportStorage*>(param);
```
The callback is invoked by libiec61850's internal network thread. The `param` is a raw pointer to `ReportStorage`. If the `DataModel` (which owns `ReportStorage`) is destroyed or replaced while a report callback is in flight, this dereferences freed memory. The invariants document says "uninstall handlers before `IedConnection_abort()`", but if the connection drops unexpectedly (network failure), libiec61850 may fire a final callback after the handler uninstall window.

**Risk:** Medium — mitigated by the uninstall-before-disconnect pattern, but not fully safe against unexpected disconnects.

---

### 13. Connection close callback — potential use-after-free
**File:** `src/libiec61850_stub/libiec61850_adapter.cpp:33-38`
```cpp
void callback_ConnectionHandler(void *param, sIedConnection *con)
{
    ApiAdapter *adapter = static_cast<ApiAdapter*>(param);
    if (adapter != nullptr) {
        adapter->callbackOnCloseEvent();
    }
}
```
`param` is `this` (ApiAdapter). If the adapter is destroyed while the connection is still active, the callback dereferences freed memory. The null check `adapter != nullptr` does not help — the pointer is non-null but invalid.

**Risk:** Low if disconnect always calls `IedConnection_abort` first, but depends on destruction order.

---

### 14. Lambda captures raw model pointer in `setRCBEnable`
**File:** `src/app/ied_backend.cpp:308-314`
```cpp
auto *model = buffered ? m_brcbComModel : m_urcbComModel;
// ...
connect(cmd.get(), &Cmd::CmdInterface::sigCmdEvent, this,
        [this, model](Cmd::CmdEvent ev) {
            if (ev.m_type == Cmd::FINISH_EVENT) {
                model->slotDataUpdated(ev.m_result);
```
The lambda captures the raw pointer `model`. The signal is delivered via `Qt::QueuedConnection` (deferred). If `slotConnected()` is called between command creation and signal delivery, the models are reallocated via `setActiveIED()` → old model objects are deleted → lambda dereferences freed pointer.

**Fix:** Capture `buffered` flag instead, re-fetch the model pointer inside the lambda: `auto *m = buffered ? m_brcbComModel : m_urcbComModel;`

---

## Suspicious / Fragile Code

### 15. `sprintf` with unbounded string input
**File:** `src/libiec61850_stub/ied_model_api_impl.cpp:193`
```cpp
char dataSetRef[130] = { 0 };
sprintf(dataSetRef, "%s.%s", lnRef.toStdString().data(), dsName);
```
If `lnRef` + `dsName` exceeds 129 characters, this is a buffer overflow. IEC 61850 references can be long with deeply nested LDs/LNs.

**Fix:** Use `QString::asprintf` or `snprintf`.

---

### 16. Download handler emits signal from C callback thread
**File:** `src/libiec61850_stub/ied_fs_api_impl.cpp:59`
```cpp
emit ctx->api->sigDownloadProgress(perc);
```
This emit happens inside a C callback from libiec61850 (runs on its internal thread). If the signal is connected with `Qt::AutoConnection` (default), Qt will detect cross-thread delivery and queue it. But if it's `DirectConnection`, this invokes the slot on the wrong thread. Currently safe because the signal chain uses queued connections, but fragile.

---

### 17. `fetchDataModel` — duplicate `getValsForLN` call
**File:** `src/libiec61850_stub/ied_model_api_impl.cpp:114-125`
```cpp
auto updVals = m_api.state().getValsForLN(builder.lastLN());
updVals->update();
// ... fetches DS, RCB, GOCB, SVCB ...
m_api.m_state.getValsForLN(builder.lastLN());  // line 125: result discarded
```
Line 125 calls `getValsForLN` again but discards the result. This makes a redundant set of network reads for every LN during model building, roughly doubling the connect time.

**Fix:** Remove line 125.
