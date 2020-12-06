from fastapi.applications import FastAPI

app = FastAPI()

@app.get("/")
def index():
    return "Hello, world!"

if __name__ == "__main__": 
    import uvicorn

    uvicorn.run(app)
    