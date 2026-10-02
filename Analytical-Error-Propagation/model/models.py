from typing import Dict, List, Tuple, Union

from pydantic import BaseModel, Field


class ComponentVariant(BaseModel):
    name: str
    op: str = "add"
    me: float = 0.0
    error: float = 0.0


class BenchmarkNode(BaseModel):
    id: str
    op: str
    inputs: List[str]
    output: str


class BenchmarkDFG(BaseModel):
    inputs: List[str]
    input_ranges: Dict[str, Tuple[Union[int, float], Union[int, float]]] = Field(default_factory=dict)
    nodes: List[BenchmarkNode]
    output: str
