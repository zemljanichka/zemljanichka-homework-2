#include "trie.h"
#include <cassert>
#include <queue>

using namespace std;

trie_t::trie_t() : tree_size(0), string_end_count(0) {}

trie_t::~trie_t() = default;

trie_t::trie_t(const trie_t &other)
    : tree_size(other.tree_size), string_end_count(other.string_end_count), children(other.children) {}

trie_t &trie_t::operator=(const trie_t &other) {
  tree_size = other.tree_size;
  string_end_count = other.string_end_count;
  children = other.children;
  return *this;
}

void trie_t::insert(const string &str) {
  trie_t *root = this;
  ++root->tree_size;
  for (int chr = 0; chr < str.size(); chr++) {
    root = &children[str[chr]];
    ++root->tree_size;
  }
  ++root->string_end_count;
}

size_t trie_t::size() const {
  return tree_size;
}

bool trie_t::empty() const {
  return tree_size == 0;
}

void trie_t::clear() {
  tree_size = 0;
  string_end_count = 0;
  children.clear();
}

bool trie_t::erase(const string &str) {
  trie_t *root = this;
  if (!root->find(str))
    return false;

  --root->tree_size;
  for (int chr = 0; chr < str.size(); chr++) {
    root = &children[str[chr]];
    --root->tree_size;
    if (root->tree_size == 0) {
      root->clear();
      return true;
    }
  }
    --root->string_end_count;
    return true;
  }


bool trie_t::find(const string &str) const {
  auto root = check_in(str);
  if (root || root->string_end_count != 0)
    return false;
  return true;
}

size_t trie_t::count_with_prefix(const string &prefix) const {
  auto root = check_in(prefix);
  if (!root)
    return 0;
  return root->tree_size;
}

string trie_t::operator[](size_t index) const {
  assert(index < tree_size);
  string result_string;
  const trie_t *root = this;
  while (root) {
    if (index < root->string_end_count) {
      break;
    }
    index -= root->string_end_count;
    auto temp = root;
    root = nullptr;
    for (const auto &[child, next_root] : temp->children) {
      if (index < next_root.tree_size) {
        result_string += child;
        root = &next_root;
        break;
      }
      index -= next_root.tree_size;
    }
  }

  return result_string;
}

vector<string> trie_t::to_vector() const {
  /* bfs */
  queue<pair<const trie_t &, size_t>> char_queue;
  vector<string> result_vector(tree_size);
  char_queue.push({*this, 0});
  while (!char_queue.empty()) {
    auto [new_root, start_id] = char_queue.front();
    char_queue.pop();
    start_id += new_root.string_end_count;
    for (const auto &[chr, next_root] : new_root.children) {
      char_queue.push({next_root, start_id});
      for (int i = 0; i < next_root.tree_size; ++i) {
        result_vector[start_id++] += chr;
      }
    }
  }

  return result_vector;
}

void trie_t::swap(trie_t &other) {
  std::swap(tree_size, other.tree_size);
  std::swap(string_end_count, other.string_end_count);
  std::swap(children, other.children);
}

const trie_t *trie_t::check_in(const string &str) const {
  const trie_t *root = this;
  for (int chr = 0; chr < str.size(); chr++) {
    auto new_root = children.find(str[chr]);
    if (new_root == root->children.end()){
      return nullptr;
    }
    root = &new_root->second;
  }
  return root;
}