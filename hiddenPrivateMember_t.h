#ifndef HIDDEN_PRIVATE_MEMBER_T_H
#define HIDDEN_PRIVATE_MEMBER_T_H


// Useful for having members inside of classes that that class cannot access, but allow other  
template <typename T, typename permitted_t>
class hiddenPrivateMember_t
{
	private:
		T data;
		T get() { return data; }
		void set(T& new_data) { data = new_data; }

		friend permitted_t;
};

#endif