int test();
int dup();
int main() {
return test() - dup();
}

int dup() {
int a;
int b;
a = 5;
b = 5;
if (a && b == 5) {
	return 2;	
} else {
	if (a || b) {
		return 7;
	} else {
	return 5;
	}
	return 8;
}
return 3;
}
