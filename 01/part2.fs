[<EntryPoint>]
let main args =
    let freqChanges =
        args
        |> Array.head
        |> System.IO.File.ReadLines
        |> Seq.map int
        |> Seq.toArray |> (fun s -> seq { while true do yield! s })

    let repeatedFreq = seq {
        let mutable soFar = Set.empty
        let mutable freq  = 0
        for c in freqChanges do
            freq <- freq + c
            if Set.contains freq soFar then yield freq
            else soFar <- Set.add freq soFar
    }

    repeatedFreq |> Seq.head |> printfn "%i" |> (fun x -> 0)