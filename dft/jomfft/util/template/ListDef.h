struct LIST;
struct LIST_(Node);

struct LIST *LIST_(new)();

void LIST_(delete)(struct LIST *p);

void LIST_(clear)(struct LIST *p);

void LIST_(append)(struct LIST *p, VALUE_TYPE *x);

struct LIST_(Node) *LIST_(head)(struct LIST *p);

struct LIST_(Node) *LIST_(next)(struct LIST_(Node) *p);

VALUE_TYPE *LIST_(data)(struct LIST_(Node) *p);
