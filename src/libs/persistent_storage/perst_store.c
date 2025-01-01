#include "perst_store.h"

// detect errors and default a section to factory/0 if corrupted
// manage changes/additions to the data in section after a software upgrade
// manage sections metadata
// should not corrupt if powered down during write to physical media

// split related data into section
// section should be oversize to accomodate growth
// use a CRC on the data of each section to detect corruption
// current size of a section is stored along the data and included in checksum
// a schema/version id is stored

// on init, read each section in RAM and verify checksum. If corrupted, revert to default (factory/0)
// when app update a section, app is responsible to commit to memory