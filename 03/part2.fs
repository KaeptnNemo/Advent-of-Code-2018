open System.Text.RegularExpressions
open System.IO

type Rect = { Id: int; X1: int; Y1: int; X2: int; Y2: int }
with
    static member create (arr: int array) ={
        Id = arr.[0]
        X1 = arr.[1]
        Y1 = arr.[2]
        X2 = arr.[1] + arr.[3] - 1
        Y2 = arr.[2] + arr.[4] - 1
    }

    static member overlaps (r1: Rect) (r2: Rect) =
        let xGap = (r1.X1 > r2.X2 || r2.X1 > r1.X2)
        let yGap = (r1.Y2 < r2.Y1 || r2.Y2 < r1.Y1)
        let isSame = r1.Id = r2.Id
        not (xGap || yGap || isSame)

[<EntryPoint>]
let main args =
    let groups line =
        let reg = Regex "#([0-9]+) @ ([0-9]+),([0-9]+): ([0-9]+)x([0-9]+)"
        let m   = reg.Match line
        m.Groups |> Seq.tail |> Seq.map (fun g -> g.ToString() |> int) |> Seq.toArray

    let rects : Rect array =
        "input.txt"
        |> File.ReadLines
        |> Seq.map (fun line -> groups line)
        |> Seq.map Rect.create
        |> Seq.toArray

    let overlapsAny r =
        rects
        |> Array.map (Rect.overlaps r)
        |> Array.fold (fun f n -> f || n) false

    let flags = Array.map (fun r -> (r.Id,(overlapsAny r))) rects

    flags |> Seq.ofArray |> Seq.filter (fun (i,f) -> not f) |> Seq.head |> fst |> printfn "%A" 
    0