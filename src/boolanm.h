#ifndef BOOLANIM_H_
#define BOOLANIM_H_

class BoolAnim {
private:
	mutable float value;
	mutable float trans_dir;	// transition direction (sign)
	long trans_start;	// transition start time
	long trans_dur;

	long (*get_msec)();

	void update(long tm) const;

public:
	BoolAnim(bool st = false);

	void set_transition_duration(long dur);
	void set_time_callback(long (*time_func)());

	void change(bool st);
	void change(bool st, long trans_start);

	bool get_state() const;
	bool get_state(long tm) const;

	float get_value() const;
	float get_value(long tm) const;

	// transition direction (-1, 0, 1)
	float get_dir() const;
	float get_dir(long tm) const;

	operator bool() const;	// equivalent to get_state
	operator float() const;	// equivalent to get_value
};

#endif	// BOOLANIM_H_
