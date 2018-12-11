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

    member self.InnerPoints() = seq {
        for x in self.X1 .. self.X2 do
            for y in self.Y1 .. self.Y2 do
                yield (x,y)
    }

[<EntryPoint>]
let main args =
    let groups line =
        let reg = Regex "#([0-9]+) @ ([0-9]+),([0-9]+): ([0-9]+)x([0-9]+)"
        let m   = reg.Match line
        m.Groups |> Seq.tail |> Seq.map (fun g -> g.ToString() |> int) |> Seq.toArray

    let rects =
        "input.txt"
        |> File.ReadLines
        |> Seq.map (fun line -> groups line)
        |> Seq.map Rect.create

    let fabric = Array2D.init<int> 1000 1000 (fun x y -> 0)
    for rect in rects do
        for (x,y) in rect.InnerPoints() do
            fabric.[x,y] <- fabric.[x,y] + 1

    let fabricSeq = seq {
        for x in 0..999 do
            for y in 0..999 do
                yield fabric.[x,y]
    }
    fabricSeq |> Seq.filter (fun x -> x > 1) |> Seq.length |> printfn "%i"
    0