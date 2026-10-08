import math
import random

# Reward of a configuration over the error cap. Milder than the original -5, so the search
# keeps exploring near the cap instead of retreating from it.
OVER_CAP_REWARD = -2.0


class Node:
    def __init__(self, node_id, code, parent, name=None):
        self.node_id = node_id
        self.code = code
        self.name = name
        self.parent = parent
        self.children = []
        self.wins = 0.0
        self.visits = 0
        self.uct = 0.0
        self.dead = False
        self.rms = None
        self.area = None
        self.level = 0 if parent is None else parent.level + 1

    def decided(self):
        used = set()
        n = self
        while n.parent is not None:
            used.add(n.node_id)
            n = n.parent
        return used

    def codes(self, base):
        codes = dict(base)
        n = self
        while n.parent is not None:
            codes[n.node_id] = n.code
            n = n.parent
        return codes

    def path(self):
        parts = []
        n = self
        while n.parent is not None:
            parts.append(f"{n.node_id}={n.name}")
            n = n.parent
        return ", ".join(reversed(parts)) if parts else "(root)"


def backprop(node, reward, scaler):
    n = node
    while n.parent is not None:
        n.wins += reward
        n.visits += 1
        explore = math.sqrt(math.log(n.parent.visits + 1) / (n.visits + 1))
        n.uct = n.wins / (n.visits + 1) + scaler * explore
        n = n.parent
    n.wins += reward
    n.visits += 1


def run_mcts(candidates, variants, base, error_fn, area_fn, cap, budget, scaler=1.0, on_step=None):
    # variants: {node_id: [(code, name), ...]}
    # base:     {node_id: code}, the starting configuration
    # on_step:  on_step(it, child, best) after every expansion
    base_area = area_fn(base)
    root = Node(None, None, None)
    active = [root]
    best = None
    expanded = 0

    it = 0
    while it <= budget and active:
        cur = max(active, key=lambda n: n.uct)

        tried = {(c.node_id, c.code) for c in cur.children}
        if cur.children:
            locked = cur.children[0].node_id
            avail = [(locked, code, name) for code, name in variants[locked] if (locked, code) not in tried]
        else:
            used = cur.decided()
            avail = [(nid, code, name) for nid in candidates if nid not in used for code, name in variants[nid]]
        if not avail:
            active.remove(cur)
            it += 1
            continue

        nid, code, name = random.choice(avail)
        child = Node(nid, code, cur, name)
        cur.children.append(child)
        expanded += 1
        codes = child.codes(base)
        child.rms = error_fn(codes)

        if child.rms <= cap:
            child.area = area_fn(codes)
            reward = max(0.0, 1.0 - child.area / base_area)
            active.append(child)
            if child.area < base_area and (best is None or child.area < best.area):
                best = child
        else:
            child.dead = True
            reward = OVER_CAP_REWARD

        if on_step:
            on_step(it, child, best)

        backprop(child, reward, scaler)
        it += 1

    stats = {"base_area": base_area, "iterations": it, "expanded": expanded}
    if best is None:
        return {"codes": dict(base), "rms": error_fn(base), "area": base_area, "path": "(none)", **stats}
    return {"codes": best.codes(base), "rms": best.rms, "area": best.area, "path": best.path(), **stats}
