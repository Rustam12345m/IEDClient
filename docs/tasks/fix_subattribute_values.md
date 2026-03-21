# Bug: SubAttribute values empty or misassigned in tree and state views

## Root cause (confirmed by debug trace)

The model tree mixes children from **multiple FCs** under the same parent node. For example, `phsA` has 4 children: `d[DC]`, `cVal[MX]`, `q[MX]`, `t[MX]`. When values are read with FC=MX, the IED returns a structure with 3 elements (`cVal, q, t`). But `getValuesForItemByMmsValue` uses **positional matching**:

```
MMS element 0 (cVal data) → model child 0 (d)     ← WRONG!
MMS element 1 (q data)    → model child 1 (cVal)   ← WRONG!
MMS element 2 (t data)    → model child 2 (q)       ← WRONG!
model child 3 (t)         → skipped
```

The `d[DC]` attribute exists in the model but isn't present in the MMS MX read, causing all subsequent elements to be shifted by one position. Values get assigned to wrong model items, resulting in empty or incorrect display.

Debug output:
```
MISMATCH: MMXU1.A.phsA mms=3 model=4 (using 3)  ← 3 MMS elements, 4 model children
MISMATCH: MMXU1.A.phsA.d mms=1 model=0 (using 0) ← d has MMS data but 0 model children
```

## Fix approach

### Option A: Read each leaf individually (safest, slower)
Instead of reading the DA structure and walking it positionally, read each leaf attribute individually using `IedConnection_readObject(ref, fc)`. This avoids positional mismatches entirely but increases the number of MMS requests.

### Option B: Match by name instead of position (correct, moderate complexity)
When walking the MMS structure, match MMS elements to model children by name instead of by index. Requires getting MMS variable names from the structure, which the current MMS API doesn't directly expose for structure elements.

### Option C: Filter model children by FC during value assignment (simplest fix)
When walking the MMS structure with a specific FC, skip model children whose FC doesn't match. This keeps positional matching but only considers children from the same FC:

```cpp
case MMS_STRUCTURE: {
    int mmsIdx = 0;
    int count = MmsValue_getArraySize(mmsValue);

    for (int i = 0; i < item->getItemCount() && mmsIdx < count; i++) {
        auto child = item->getItem(i);
        auto *da = dynamic_cast<Core::DataAttribute*>(child.get());

        // Skip children from different FCs
        if (da && da->fcNum() != currentFC) {
            continue;
        }

        getValuesForItemByMmsValue(child, vals, MmsValue_getElement(mmsValue, mmsIdx));
        mmsIdx++;
    }
    break;
}
```

**Problem with Option C**: `getValuesForItemByMmsValue` doesn't know the current FC. Would need to pass it through the recursion.

### Recommended: Option C with FC propagation
Pass the FC through the recursive chain. `getValuesForDataAttribute` already knows the FC. Thread it through to `getValuesForItemByMmsValue` so it can skip non-matching children.

## Files to modify
- `src/libiec61850_stub/ied_state_api_impl.cpp` — add FC parameter to `getValuesForItemByMmsValue`, filter children by FC in STRUCTURE case

## Other fixes in this change
- `ln_common_tree.cpp` — use `layoutChanged()` instead of flat `dataChanged` for nested tree refresh
- `ied_model_tree.cpp` / `ln_common_tree.cpp` — always show `getValue()` regardless of child count (already done)
- Remove debug `fprintf` traces after fix is verified
