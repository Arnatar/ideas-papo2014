create_headers() {
  f=$1
  cproto $1.c > $1.h
}

create_headers lib/helpers
create_headers logic
