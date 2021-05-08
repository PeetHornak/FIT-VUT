import mpld3
from fastapi import FastAPI, Request
from fastapi.responses import HTMLResponse
from fastapi.templating import Jinja2Templates
from upa.database import query

app = FastAPI()
templates = Jinja2Templates(directory=".")


@app.get("/", response_class=HTMLResponse)
async def read_item(request: Request):
    query1 = mpld3.fig_to_html(query.characteristics_query())
    query2 = mpld3.fig_to_html(query.query2())
    query3 = mpld3.fig_to_html(query.asn_query())
    return templates.TemplateResponse(
        "index.html", {"request": request, "query1": query1, "query2": query2, "query3": query3}
    )
