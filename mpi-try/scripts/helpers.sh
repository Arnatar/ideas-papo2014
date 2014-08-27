pprint() {
  echo $1 | sed -r ':L;s=\b([0-9]+)([0-9]{3})\b=\1.\2=g;t L'
}
