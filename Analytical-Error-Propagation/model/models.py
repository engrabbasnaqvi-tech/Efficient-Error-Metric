
from pydantic import BaseModel, Field


class ComponentVariant(BaseModel):
    name: str
    op: str = "add"
    me: float = 0.0
    error: float = 0.0


class BenchmarkNode(BaseModel):
    id: str
    op: str
    inputs: list[str]
    output: str


class BenchmarkDFG(BaseModel):
    inputs: list[str]
    input_ranges: dict[str, tuple[int | float, int | float]] = Field(default_factory=dict)
    nodes: list[BenchmarkNode]
    output: str
