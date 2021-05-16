#include<cstdio>
#include<cstdint>
#include<vector>
#include<algorithm>
//#define DEBUG

using namespace std;

struct cidr{
	uint32_t value;
	uint32_t mask;
	void read(FILE* f);
	void print(FILE* f);
	void reduce(uint32_t new_mask);
	static uint32_t get_mask(int l);
	bool operator==(const cidr& another)const{
		return value == another.value && mask == another.mask;
	}
	bool operator<(const cidr& another)const{
		return value < another.value;
	}
};

uint32_t cidr::get_mask(int l){
	uint32_t m = (int)((1ll<<l)-1);
	while(!(m& (1<<31)))
		m<<= 1;
	return m;
}

void cidr::read(FILE* f){
	int a, b, c, d, l;
	fscanf(f, "%d.%d.%d.%d/%d\n", &a, &b, &c, &d, &l);
	value = (a<<24) | (b<<16) | (c<<8) | d;
	mask = get_mask(l);
#ifdef DEBUG
	printf("CIDR: %d.%d.%d.%d/%d\n", a, b, c, d, l);
	printf("CIDRV: 0x%08x 0x%08x\n", value, mask);
#endif
}

void cidr::print(FILE* f){
	int a, b, c, d, l, m;
	a = (value >> 24) & 255;
	b = (value >> 16) & 255;
	c = (value >> 8) & 255;
	d = (value) & 255;
	for(m = mask, l = 0; m; l++){
		m<<=1;
	}
	fprintf(f, "%d.%d.%d.%d/%d\n", a, b, c, d, l);
}

void cidr::reduce(uint32_t new_mask){
	mask = new_mask;
	value &= mask;
}

cidr merge(cidr a, cidr b){
	for(int ml = 24; ml >= 8; ml--){
		auto mask = cidr::get_mask(ml);
		a.reduce(mask); b.reduce(mask);
		if(a == b)
			return a;
	}
	a.value = 0; a.mask = 0;
	return a;
}

vector<cidr> merge_cidrs(vector<cidr> s){
	bool merged = false;
	int i;
	vector<cidr> result;
	for(i = 1; i < s.size(); i++){
		if(merge(s[i-1], s[i]).value){
			result.push_back(merge(s[i-1], s[i]));
			i++;
			merged = true;
		}else{
			result.push_back(s[i-1]);
			if(i == s.size()-1)
				result.push_back(s[i]);
		}
	}
	return merged? merge_cidrs(result): s;
}

vector<cidr> read_cidrs(FILE* f){
	vector<cidr> result;
	while(!feof(f)){
		cidr a;
		a.read(f);
		result.push_back(a);
	}
	return result;
}


int main(){
	auto s = read_cidrs(stdin);
	stable_sort(s.begin(), s.end());
	s = merge_cidrs(s);
	for(auto& item: s){
		item.print(stdout);
	}
}
