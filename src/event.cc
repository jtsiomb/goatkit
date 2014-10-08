#include "event.h"


const char *goatkit::event_type_name(EventType type)
{
	// XXX keep this array synchronized with enum EventType
	static const char *names[NUM_EVENTS] = {
		"mouse button",
		"mouse motion",
		"focus",
		"keyboard",
		"click",
		"double click",
		"change"
	};

	return names[type];
}
