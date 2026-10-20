"""
Terminal output of pipeline.py
"""
import math
import time
from contextlib import contextmanager
from decimal import ROUND_HALF_UP, Decimal
from pathlib import Path

from rich import box
from rich.console import Console, Group
from rich.live import Live
from rich.padding import Padding
from rich.panel import Panel
from rich.progress import BarColumn, Progress, SpinnerColumn, TextColumn
from rich.rule import Rule
from rich.table import Table
from rich.text import Text
from rich.theme import Theme

COLOURS = {
    "accent": "#7aa2f7",
    "muted": "#8b93a7",
    "faint": "#3b4252",
    "good": "#9ece6a",
    "teal": "#73daca",
    "warn": "#e0af68",
    "bad": "#f7768e",
    "violet": "#bb9af7",
    "orange": "#ff9e64",
}
THEME = Theme({**COLOURS, "title": f"bold {COLOURS['accent']}", "path": f"{COLOURS['muted']} italic"})
STAGE_COLOURS = {
    "setup": "muted",
    "analytical search": "violet",
    "handoff check": "teal",
    "verification": "teal",
    "eem search": "violet",
    "simulation search": "accent",
    "final simulation": "good",
    "synthesis": "orange",
}
SEP = ("  ·  ", "faint")
NAME_W = 19
INDENT = 5 + 2 + NAME_W + 11


def duration(seconds):
    if seconds is None:
        return "-"
    if seconds < 1e-3:
        return f"{seconds * 1e6:.0f} µs"
    if seconds < 1:
        return f"{seconds * 1e3:.0f} ms"
    if seconds < 60:
        return f"{seconds:.1f} s"
    if seconds < 3600:
        return f"{seconds / 60:.1f} min"
    return f"{seconds / 3600:.1f} h"


def cap_bar(value, cap, width=14):
    frac = min(value / cap, 1.0) if cap else 1.0
    colour = "bad" if value > cap else "accent" if frac < 0.8 else "warn"
    filled = round(frac * width)
    bar = Text("━" * filled, style=colour)
    bar.append("━" * (width - filled), style="faint")
    pct = value / cap * 100 if cap else 100
    bar.append(f" {math.ceil(pct) if value > cap else round(pct):3d}%", style="bad" if value > cap else "muted")
    return bar


def gap(model, simulated, cap):
    diff = float(Decimal(str(simulated - model)).quantize(Decimal("0.01"), rounding=ROUND_HALF_UP))
    return Text(f"{diff:+.2f} pp", style="bad" if simulated > cap else "muted")


def plural(count, word):
    return f"{count:,} {word}" + ("" if count == 1 else "s")


def short(path):
    try:
        return str(Path(path).resolve().relative_to(Path.cwd()))
    except ValueError:
        return str(path)


def tile(label, colour, value, style, lines):
    body = Text(value, style=style, justify="center")
    for line in lines:
        body.append("\n")
        body.append_text(line if isinstance(line, Text) else Text(line, style="muted"))
    return Panel(body, title=Text(f" {label} ", style=f"bold {COLOURS[colour]}"), title_align="left",
                 box=box.ROUNDED, border_style="faint", padding=(0, 1))


class PipelineConsole:
    def __init__(self):
        self.console = Console(theme=THEME, record=True)
        if not self.console.is_terminal:
            self.console.width = 140
        self.live = Console(theme=THEME, stderr=True)
        self.drawing = self.live.is_terminal
        self.number = 0

    def print(self, *objects, **kwargs):
        self.console.print(*objects, **kwargs)

    def error(self, message):
        self.console.print(Text.assemble(("error: ", "bold bad"), message))

    def save(self, path):
        self.console.save_text(path)

    # ---- configuration -------------------------------------------------------
    def header(self, fields, subtitle, warnings=()):
        rows = [[], [], []]
        for i, (label, value, note) in enumerate(fields):
            if i:
                for row in rows:
                    row.append(Text(" │ ", style="faint"))
            rows[0].append(Text(label, style="muted"))
            rows[1].append(Text(value, style="bold"))
            rows[2].append(Text(note, style="muted"))
        widths = [max(len(rows[r][c].plain) for r in range(3)) for c in range(len(rows[0]))]
        grids = []
        if sum(widths) <= self.console.width - 8:
            grids.append(rows)
        else:
            half = (len(fields) + 1) // 2 * 2 - 1
            grids.append([row[:half] for row in rows])
            grids.append([row[half + 1:] for row in rows])
        body = []
        for part in grids:
            grid = Table.grid(padding=0)
            for _ in part[0]:
                grid.add_column(no_wrap=True)
            for row in part:
                grid.add_row(*row)
            body.append(grid)
        for warning in warnings:
            body.append(Text(warning, style="warn"))
        self.console.print()
        self.console.print(Panel(Group(*body), title=Text(" Hybrid DSE ", style="title"), title_align="left",
                                 subtitle=Text(f" {subtitle} ", style="muted"), subtitle_align="right",
                                 box=box.ROUNDED, border_style="accent", padding=(0, 2)))
        self.console.print()

    # ---- stages --------------------------------------------------------------
    def line(self, name, seconds, detail, sub=None, warn=False):
        self.number += 1
        mark = ("! ", "warn") if warn else ("✓ ", "good")
        self.console.print(Text.assemble((f"  {self.number}  ", "accent"), mark, (f"{name:<{NAME_W}}", "bold"),
                                         (f"{duration(seconds):>8}   ", "muted"), detail))
        for text in [sub] if isinstance(sub, (str, Text)) else sub or []:
            self.console.print(Text(" " * INDENT) + (text if isinstance(text, Text) else Text(text, style="muted")))

    def gap_line(self):
        self.console.print()

    @contextmanager
    def working(self, message, colour="accent"):
        if not self.drawing:
            yield
            return
        with self.live.status(Text(f"  {message} ...", style="muted"), spinner="dots", spinner_style=colour):
            yield

    def setup(self, seconds, source, start, first):
        detail = Text.assemble(("model ", "muted"), (source, "bold"))
        if start["saving_pct"] > 0:
            sub = Text.assemble(("start ", "muted"), (f"{start['saving_pct']:.1f} %", "good"), (" saving at ", "muted"),
                                (f"{start['rms']:.3f} %", "bold"), (f" ({first})", "muted"))
        else:
            sub = Text("start all exact", style="muted")
        self.line("Setup", seconds, detail, sub)

    # ---- analytical search ---------------------------------------------------
    @contextmanager
    def seeds(self, seeds, budget, jobs):
        if not self.drawing:
            yield lambda seed, it, best: None, lambda seed: None
            return
        progress = Progress(TextColumn("     "), SpinnerColumn(style="violet", finished_text=Text("✓", style="good")),
                            TextColumn("[bold]{task.fields[label]}[/bold]"),
                            BarColumn(bar_width=30, style="faint", complete_style="violet", finished_style="good"),
                            TextColumn("[muted]{task.completed:>7,.0f}/{task.total:,.0f}[/muted]"),
                            TextColumn("{task.fields[best]}"), console=self.live, transient=True)
        width = len(str(max(seeds)))
        shown = list(seeds)[:24]
        tasks = {seed: progress.add_task("", total=budget, label=f"seed {seed:<{width}}",
                                         best="[muted]running[/muted]" if jobs > 1 else "[muted]waiting[/muted]")
                 for seed in shown}
        more = progress.add_task("", total=len(seeds), label=f"all {len(seeds)}", best="") if len(seeds) > len(shown) else None
        finished = []

        def step(seed, it, best):
            if seed in tasks and (it % 25 == 0 or it >= budget):
                text = ("[muted]no design under the cap yet[/muted]" if best is None else
                        f"[muted]best[/muted] [good]{best[0]:4.1f} %[/good] [muted]at {best[1]:.2f} %[/muted]")
                progress.update(tasks[seed], completed=min(it, budget), best=text)

        def done(seed):
            finished.append(seed)
            if seed in tasks:
                progress.update(tasks[seed], completed=budget)
            if more is not None:
                progress.update(more, completed=len(finished))

        with progress:
            yield step, done

    def analytical(self, phase, start_savings):
        cand = phase["candidates"]
        detail = Text.assemble((f"{phase['seeds']} seeds", "bold"), (" → ", "muted"), (f"{len(cand)} candidates", "bold"),
                               ("   saving ", "muted"),
                               (f"{min(c['saving_pct'] for c in cand):.1f}–{max(c['saving_pct'] for c in cand):.1f} %", "good"),
                               ("   model RMS ", "muted"),
                               (f"{min(c['rms'] for c in cand):.2f}–{max(c['rms'] for c in cand):.2f} %", "bold"))
        sub = [f"{phase['iterations']:,} iterations · {phase['evaluations']:,} model evaluations · "
               f"{phase['cache_hits']:,} cached" + (f" · {phase['jobs']} processes" if phase.get("jobs", 1) > 1 else "")]
        if start_savings and max(start_savings) > 0:
            sub.append(f"random starts save {min(start_savings):.1f}–{max(start_savings):.1f} %")
        self.line("Analytical search", phase["seconds"], detail, sub)
        self.gap_line()

    # ---- hand-off check / verification ---------------------------------------
    def _candidate_table(self):
        table = Table(box=box.SIMPLE_HEAD, header_style="muted", border_style="faint", pad_edge=False,
                      padding=(0, 1), show_edge=False)
        for column, justify in (("#", "right"), ("Saving", "right"), ("Model RMS", "right"), ("Simulated", "right"),
                                ("Gap", "right"), ("", "left"), ("", "right")):
            table.add_column(column, justify=justify)
        return table

    @contextmanager
    def candidates(self, cap):
        table = self._candidate_table()
        rows = []
        view = Padding(table, (0, 0, 0, INDENT))
        live = Live(view, console=self.live, transient=True, refresh_per_second=8) if self.drawing else None

        def add(cand, chosen, seconds):
            ok = cand["sim_rms"] <= cap
            label = Text("chosen", style="good") if chosen else Text("over cap", style="bad") if not ok else Text("")
            row = (str(len(rows) + 1), f"{cand['saving_pct']:.1f} %", f"{cand['rms']:.3f} %", f"{cand['sim_rms']:.3f} %",
                   gap(cand["rms"], cand["sim_rms"], cap), label, Text(duration(seconds), style="muted"))
            rows.append(row)
            table.add_row(*row)
            if live:
                live.update(view)

        def note(text):
            rows.append(None)
            table.add_row("", Text(text, style="muted"), "", "", "", "", "")

        if live:
            with live:
                yield add, note
        else:
            yield add, note
        self.table = table

    def verified(self, name, seconds, n_candidates, simulated, cap, chosen, fallback):
        over = sum(c["sim_rms"] > cap for c in simulated)
        if chosen:
            detail = Text.assemble((f"{len(simulated)} of {n_candidates}", "bold"), (" candidates simulated", "muted"),
                                   (f", {over} over the cap" if over else "", "bad"), SEP, ("chosen ", "muted"),
                                   (f"{chosen['saving_pct']:.1f} %", "good"), (" saving at ", "muted"),
                                   (f"{chosen['sim_rms']:.3f} %", "bold"))
            self.line(name, seconds, detail)
        else:
            source, saving = fallback
            verb = "continues from" if name == "Hand-off check" else "falls back to"
            how = (f"{verb} the random start, {saving:.1f} % saving" if source == "start"
                   else f"{verb} the exact circuit")
            detail = Text.assemble(("none of ", "warn"), (f"{len(simulated)}", "bold"), (" under the cap", "warn"),
                                   SEP, (how, "muted"))
            self.line(name, seconds, detail, warn=True)
        self.console.print(Padding(self.table, (0, 0, 0, INDENT)))
        self.gap_line()

    # ---- MCTS search ---------------------------------------------------------
    @contextmanager
    def search(self, evaluator, budget, exact_area, quiet):
        if not quiet:
            def line(it, child, best):
                move = f"{child.node_id}={child.name}"
                status = (Text(f"RMS {child.rms:7.3f} %  over cap", style="bad") if child.dead else
                          Text.assemble((f"RMS {child.rms:7.3f} %", "good"), (f"  area {child.area:9.1f}", "")))
                self.console.print(Text.assemble((f"     iter {it:>6}  level {child.level:>3}  ", "muted"), status,
                                                 (f"  {move}", "muted"), ("  new best" if best is child else "", "bold good")))
            yield line
            return
        if not self.drawing:
            yield None
            return
        progress = Progress(TextColumn("     "), SpinnerColumn(style="accent", finished_text=Text("✓", style="good")),
                            BarColumn(bar_width=34, style="faint", complete_style="accent", finished_style="good"),
                            TextColumn("[muted]{task.completed:>7,.0f}/{task.total:,.0f}[/muted]"),
                            TextColumn("{task.fields[best]}"), TextColumn("[muted]{task.fields[info]}[/muted]"),
                            console=self.live, transient=True)
        task = progress.add_task("", total=budget, best="", info="")
        snap = evaluator.snapshot()
        started = time.perf_counter()

        def step(it, child, best):
            if it % 5 and it < budget:
                return
            evals = evaluator.since(snap)
            per = evals["seconds"] / evals["calls"] if evals["calls"] else None
            left = (time.perf_counter() - started) / max(it, 1) * (budget - it)
            text = ("[muted]no design under the cap yet[/muted]" if best is None else
                    f"[muted]best[/muted] [good]{(1 - best.area / exact_area) * 100:.1f} %[/good] "
                    f"[muted]at[/muted] {best.rms:.3f} %")
            info = f"{evals['calls']:,} {evaluator.name} evaluations · {duration(per)} each · ~{duration(left)} left"
            progress.update(task, completed=min(it, budget), best=text, info=info)

        with progress:
            yield step

    def searched(self, name, phase, start_saving):
        per = phase["eval_seconds"] / phase["evaluations"] if phase["evaluations"] else None
        kind = "simulations" if phase["evaluator"] == "simulation" else f"{phase['evaluator']} evaluations"
        detail = Text.assemble(("best ", "muted"), (f"{phase['rms']:.3f} %", "bold"), SEP,
                               (f"{phase['saving_pct']:.1f} %", "good"), (" saving", "muted"),
                               (f"   {phase['saving_pct'] - start_saving:+.1f} over its start", "muted"))
        sub = (f"{phase['iterations']:,} iterations (budget {phase['budget']:,}) · {phase['evaluations']:,} {kind} · "
               f"{phase['cache_hits']:,} cached · {duration(per)} each")
        self.line(name, phase["seconds"], detail, sub)
        self.gap_line()

    # ---- final simulation and synthesis --------------------------------------
    def final(self, final, cap, samples, seconds, searched_rms):
        detail = Text.assemble((f"{final['sim_rms']:.3f} %", "bold"), (f" of the {cap:g} % cap   ", "muted"),
                               cap_bar(final["sim_rms"], cap))
        sub = None
        if searched_rms is not None and abs(searched_rms - final["sim_rms"]) >= 1e-9:
            sub = Text(f"the search gave {searched_rms:.3f} %", style="warn")
        elif searched_rms is not None:
            sub = f"{samples:,} samples, same as during the search"
        self.line("Final simulation", seconds, detail, sub)

    def synthesised(self, report, seconds):
        self.gap_line()
        exact, final = report["exact"], report["final"]
        if report["area_reduction_pct"] is None:
            self.line("Synthesis", seconds, Text("Design Compiler did not finish", style="bad"),
                      [Text(f"{name}: see {run['folder']}/dc.log", style="bad") for name, run in report.items()
                       if name in ("exact", "final") and not run["ran"]], warn=True)
            return
        slack = final["slack"]
        detail = Text.assemble(("area ", "muted"), (f"{report['area_reduction_pct']:.1f} %", "good"), SEP,
                               ("power ", "muted"),
                               (f"{report['power_reduction_pct']:.1f} %" if report["power_reduction_pct"] is not None else "-", "good"),
                               SEP, ("slack ", "muted"),
                               (f"{slack:+.2f} ns" if slack is not None else "-", "good" if (slack or 0) >= 0 else "bad"))

        def run(label, r):
            power = f"{r['total_mw']:.4f} mW" if r["total_mw"] is not None else "-"
            slack_text = f"{r['slack']:+.2f} ns" if r["slack"] is not None else "-"
            return f"{label} area {r['total_area']:,.1f} (cell {r['cell_area']:,.1f}) · {power} · slack {slack_text} · {duration(r['seconds'])}"

        sub = [run("exact", exact), run("final", final)]
        for name, r in (("exact", exact), ("final", final)):
            if r["slack"] is not None and r["slack"] < 0:
                sub.append(Text(f"{name}: timing violated, loosen it with --max-delay", style="warn"))
        self.line("Synthesis", seconds, detail, sub)

    # ---- results -------------------------------------------------------------
    def results(self, final, cap, synth, timing, evaluators, phases, out, log):
        c = self.console
        c.print()
        c.print(Rule(style="faint"))
        met = final["sim_rms"] <= cap
        error_lines = [cap_bar(final["sim_rms"], cap, 12)]
        if final.get("model_rms") is not None:
            error_lines.append(f"{final['model']} {final['model_rms']:.3f} % ({final['model_inaccuracy_pct']:.1f} % off)")
        tiles = [tile("Error", "good" if met else "bad", f"{final['sim_rms']:.3f} %",
                      f"bold {COLOURS['good' if met else 'bad']}", error_lines)]
        if synth and synth["area_reduction_pct"] is not None:
            tiles.append(tile("Area saving", "teal", f"{synth['area_reduction_pct']:.1f} %", f"bold {COLOURS['good']}",
                              [f"DC {synth['final']['total_area']:,.0f} of {synth['exact']['total_area']:,.0f}",
                               f"library {final['saving_pct']:.1f} %"]))
            if synth["power_reduction_pct"] is not None:
                tiles.append(tile("Power saving", "violet", f"{synth['power_reduction_pct']:.1f} %", "bold",
                                  [f"{synth['final']['total_mw']:.3f} of {synth['exact']['total_mw']:.3f} mW",
                                   f"slack {synth['final']['slack']:+.2f} ns" if synth["final"]["slack"] is not None else ""]))
        else:
            tiles.append(tile("Area saving", "teal", f"{final['saving_pct']:.1f} %", f"bold {COLOURS['good']}",
                              [f"library {final['area']:,.0f} of {final['exact_area']:,.0f}", "no synthesis"]))
        sim = evaluators["simulation"]
        lines = [plural(sim.calls, "simulation")]
        for name, ev in evaluators.items():
            if name != "simulation" and ev.calls:
                lines.append(plural(ev.calls, f"{name} evaluation"))
        tiles.append(tile("Run", "accent", duration(timing["total"]), "bold", lines))
        grid = Table.grid(expand=True, padding=(0, 1))
        for _ in tiles:
            grid.add_column(ratio=1)
        grid.add_row(*tiles)
        c.print(grid)
        c.print()
        c.print(self._timing(timing, phases, evaluators))
        c.print()
        c.print(Text.assemble(("  result  ", "muted"), (short(out), "path")))
        c.print(Text.assemble(("  log     ", "muted"), (short(log), "path")))
        c.print()

    def _timing(self, timing, phases, evaluators):
        total = timing["total"]
        each = {}
        for name, phase in phases.items():
            if phase["evaluations"]:
                kind = "simulations" if phase["evaluator"] == "simulation" else f"{phase['evaluator']} evaluations"
                each[f"{name} search"] = (phase["eval_seconds"] / phase["evaluations"], phase["evaluations"], kind)
        checked = sum(c.get("sim_rms") is not None for c in phases.get("analytical", {}).get("candidates", []))
        for stage in ("handoff check", "verification"):
            if stage in timing and checked:
                each[stage] = (timing[stage] / checked, checked, "simulations" if checked > 1 else "simulation")
        each["final simulation"] = (timing["final simulation"], 1, "simulation")

        table = Table(box=None, show_header=True, header_style="muted", padding=(0, 1), pad_edge=False)
        table.add_column("", width=2)
        table.add_column("", width=NAME_W)
        table.add_column("", width=42)
        table.add_column("time", justify="right", width=9)
        table.add_column("share", justify="right", width=7)
        table.add_column("  per evaluation", justify="left", no_wrap=True)
        for name, seconds in timing.items():
            if name == "total":
                continue
            colour = STAGE_COLOURS.get(name, "muted")
            frac = seconds / total if total else 0
            n = frac * 40
            bar = Text("█" * int(n), style=colour)
            rest = n - int(n)
            if rest > 0.05 or not int(n):
                bar.append("▏▎▍▌▋▊▉"[min(6, int(rest * 7))], style=colour)
            per = Text("")
            if name in each:
                mean, count, kind = each[name]
                per = Text.assemble((f"{duration(mean):>8}", "bold"), (f"  × {count:,} {kind}", "muted"))
            table.add_row(Text("■", style=colour), Text(name, style="bold"), bar, Text(duration(seconds), style="bold"),
                          Text(f"{frac * 100:.1f} %", style="muted"), per)
        table.add_row("", Text("total", style="muted"), "", Text(duration(total), style="bold accent"),
                      Text("100 %", style="muted"), "")
        parts = [Text("  time", style="muted"), Padding(table, (0, 0, 0, 2))]
        sim = evaluators["simulation"]
        others = [(name, ev) for name, ev in evaluators.items() if name != "simulation" and ev.calls]
        if sim.calls:
            avg = Text.assemble(("  average  ", "muted"), ("simulation ", "muted"), (duration(sim.mean()), "bold"),
                                (f" over {sim.calls:,}", "muted"))
            for name, ev in others:
                avg.append_text(Text.assemble(SEP, (f"{name} ", "muted"), (duration(ev.mean()), "bold"),
                                              (f" over {ev.calls:,}", "muted")))
                ratio = sim.mean() / ev.mean()
                faster = f"{ratio:,.0f}× faster" if ratio >= 1 else f"{1 / ratio:,.1f}× slower"
                avg.append_text(Text.assemble(SEP, (f"{name} ", "muted"), (faster, "bold accent")))
            parts += [Text(""), avg]
        return Group(*parts)
