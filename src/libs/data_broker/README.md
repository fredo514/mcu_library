* models are only data, no business rules other than maybe basic range checks
* models can be assigned a change threshold for notifications (e.g. at what point to call callback, on float change)
* no synchronization between models
* A consumer service must never cache model value between function calls (it's ok, for within a call, but its lifetime should be short)
* A consumer service must always check if a model is valid before using it's value
* models should be statically instanciated
* models should be initialized to their invalid state upon creation

* Model points can be any type or struct 
* Model points are type safe 
* Read/writes are atomic and thread safe 
* Change notification to readers using registered callbacks (similar to publish/subscribe) 
* Valid/invalid state separate from value 

optional read-modify-write operations (e.g. increment(step) on uint32_t)

Integrate with CLI
* ls (list all data models) 
* read <modelname> (print model struct) 
* write {name:"<modelname>", <modelmember:value>} (write to a model member) 
* touch <model> (trigger change notification) 

Use something like FNV hash of model name for indexing

serialize to/from json (e.g. CLI commands)
serialize to/from binary to read/write to/from persistent memory

write to a locked model silently fail