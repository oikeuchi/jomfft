struct LIST_(Node) {
  VALUE_TYPE *data;
  struct LIST_(Node) *next;
};

struct LIST {
  struct LIST_(Node) *head;
};

struct LIST *LIST_(new)() {
  struct LIST *out = NEW(struct LIST);
  *out = (struct LIST){.head = NULL};
  return out;
}

void LIST_(delete)(struct LIST *p) {
  if (p) {
    LIST_(clear)(p);
    DELETE(p);
  }
}

void LIST_(clear)(struct LIST *p) {
  struct LIST_(Node) *it = LIST_(head)(p);
  while (it) {
    if (it->data) {
      LIST_VALUE_DELETE(it->data);
      it->data = NULL;
    }
    struct LIST_(Node) *next = it->next;
    DELETE(it);
    it = next;
  }
  p->head = NULL;
}

struct LIST_(Node) *LIST_(head)(struct LIST *p) {
  return p->head;
}

void LIST_(append)(struct LIST *p, VALUE_TYPE *x) {
  struct LIST_(Node) *o = NEW(struct LIST_(Node));
  *o = (struct LIST_(Node)){.data = x, .next = NULL};
  struct LIST_(Node) *it = LIST_(head)(p);
  if (it == NULL) {
    p->head = o;
  } else {
    while (it->next) {
      it = it->next;
    }
    it->next = o;
  }
}

struct LIST_(Node) *LIST_(next)(struct LIST_(Node) *p) {
  return p->next;
}

VALUE_TYPE *LIST_(data)(struct LIST_(Node) *p) { return p->data; }
