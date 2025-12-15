import os

configs = []
for s in range(2, 7):
    for t in range(3, 7):
        if s <= t:
            configs.append((s, t))

u_vars = ['i', 'j', 'k', 'l', 'm']
v_vars = ['a', 'b', 'c', 'd', 'e']

def generate_files():
    for S, T in configs:
        class_name = f"K{S}{T}Store"
        filename_base = f"structure/{class_name}"

        header_content = f"""#pragma once

#include "structure/Kst.h"
#include "structure/KstStore.h"

class {class_name} : public KstStore
{{
 private:
  static constexpr int S_ = {S};
  static constexpr int T_ = {T};

  Graph edges_in_circles_;
  std::vector<Kst<S_, T_>> circles_;

 public:
  {class_name}();
  ~{class_name}() override = default;

  int getS() const override {{ return S_; }}
  int getT() const override {{ return T_; }}
  bool empty() override {{ return circles_.empty(); }}
  void clear() override;

  bool createsKst(const Graph& adj, int u, int v) const override;
  void storeKst(const Graph& adj, int u, int v) override;

  void reevalCircles(const Graph& adj) override;
  bool reflipCircle(Graph& adj, Probabilities& prob) override;
}};
"""
        with open(f"include/{filename_base}.h", "w") as f:
            f.write(header_content)

        def make_nested_loops(vars_list, limit_var, count, indent_level, inner_body):
            if count == 0:
                return inner_body
            
            lines = []
            indent = "  " * indent_level
            current_var = vars_list[0]

            lines.append(f"{indent}for (int {current_var} = 0; {current_var} < {limit_var}; ++{current_var})")
            lines.append(f"{indent}{{")

            inner_content = ""
            for d in range(1, count):
                prev_var = vars_list[d-1]
                curr_var = vars_list[d]
                sub_indent = "  " * (indent_level + d)

                type_name = "int"
                
                inner_content += f"{sub_indent}for ({type_name} {curr_var} = {prev_var} + 1; {curr_var} < {limit_var}; ++{curr_var})\n"
                inner_content += f"{sub_indent}{{\n"

            body_indent = "  " * (indent_level + count)
            indented_body = body_indent + inner_body.replace("\n", f"\n{body_indent}")
            inner_content += indented_body

            for d in range(count - 1, -1, -1):
                sub_indent = "  " * (indent_level + d)
                inner_content += f"\n{sub_indent}}}"
            
            lines.append(inner_content)
            return "\n".join(lines)

        u_check_vars = u_vars[:S-1]
        adj_checks = " && ".join([f"adj[u][node_v]"] + [f"adj[u_cands[{x}]][node_v]" for x in u_check_vars])
        
        creates_body = f"""int common_v_count = 0;
for (int node_v = 0; node_v < adj.n; ++node_v)
{{
  if (node_v == v) continue;

  if ({adj_checks})
  {{
    common_v_count++;
    if (common_v_count >= (T_ - 1)) return true;
  }}
}}"""
        creates_loops = make_nested_loops(u_vars, "u_cands_size", S-1, 1, creates_body)

        v_check_vars = v_vars[:T-1]

        u_init_list = ", ".join(["u"] + [f"u_cands[{x}]" for x in u_check_vars])
        v_init_list = ", ".join(["v"] + [f"v_cands[{x}]" for x in v_check_vars])

        store_inner_loops = f"""Kst<S_, T_> created({{{u_init_list}}},
                    {{{v_init_list}}});

for (int x = 0; x < S_; ++x)
{{
  for (int y = 0; y < T_; ++y)
  {{
    edges_in_circles_.adj[created.u_arr[x]][created.v_arr[y]]++;
  }}
}}

circles_.push_back(created);"""

        store_v_loops = make_nested_loops(v_vars, "v_cands_size", T-1, 0, store_inner_loops) # indent 0 relative to replacement

        store_u_body = f"""v_cands_size = 0;

for (int node_v = 0; node_v < adj.n; ++node_v)
{{
  if (node_v == v) continue;

  if ({adj_checks})
  {{
    v_cands[v_cands_size++] = node_v;
  }}
}}

if (v_cands_size < (T_ - 1)) continue;

{store_v_loops}"""

        store_loops = make_nested_loops(u_vars, "u_cands_size", S-1, 1, store_u_body)

        cpp_content = f"""#include "structure/{class_name}.h"

#include "config/Constants.h"
#include "structure/KstHelper.h"

{class_name}::{class_name}() : edges_in_circles_(Constants::MAX_SIZE, Constants::MAX_SIZE) {{}}

void {class_name}::clear()
{{
  circles_.clear();
  edges_in_circles_.reset();
}}

bool {class_name}::createsKst(const Graph& adj, int u, int v) const
{{
  int u_cands[Constants::MAX_SIZE];
  int u_cands_size = 0;

  for (int i = 0; i < adj.m; ++i)
  {{
    if (i != u && adj[i][v])
    {{
      u_cands[u_cands_size++] = i;
    }}
  }}

  if (u_cands_size < (S_ - 1)) return false;

{creates_loops}

  return false;
}}

void {class_name}::storeKst(const Graph& adj, int u, int v)
{{
  int u_cands[Constants::MAX_SIZE];
  int u_cands_size = 0;

  for (int i = 0; i < adj.m; ++i)
  {{
    if (i != u && adj[i][v])
    {{
      u_cands[u_cands_size++] = i;
    }}
  }}

  if (u_cands_size < (S_ - 1)) return;

  int v_cands[Constants::MAX_SIZE];
  int v_cands_size;

{store_loops}
}}

void {class_name}::reevalCircles(const Graph& adj)
{{
  KstHelper::reevalCircles<S_, T_>(circles_, edges_in_circles_, adj);
}}

bool {class_name}::reflipCircle(Graph& adj, Probabilities& prob)
{{
  return KstHelper::reflipCircle<S_, T_>(circles_, edges_in_circles_, adj, prob);
}}
"""
        with open(f"src/{filename_base}.cpp", "w") as f:
            f.write(cpp_content)
        print(f"Generated {filename_base}.h and .cpp")

if __name__ == "__main__":
    generate_files()