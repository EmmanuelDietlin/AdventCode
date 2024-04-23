using System;
using System.IO;
using System.Collections;
using System.Linq;

string path = @"..\..\..\..\input.txt";
List<string> commands = new List<string>();
using (StreamReader sr = File.OpenText(path))
{
    string s;
    while((s = sr.ReadLine()) != null)
    {
        commands.Add(s);
    }
}

Directory? root = new Directory();
root.name = "/";
Directory currentDirectory = root;
foreach (var cmd in commands)
{
    string[] s = cmd.Split(' ');
    if (s[0] == "$")
    {
        if (s[1] == "cd")
        {
            string targetDir = s[2];
            if (targetDir == "..")
            {
                if (currentDirectory.parent != null)
                    currentDirectory = currentDirectory.parent;
            }
            else if (targetDir == "/")
            {
                currentDirectory = root;
            }
            else
            {
                if (currentDirectory.GetDirectory(targetDir) == null)
                {
                    Directory dir = new();
                    dir.parent = currentDirectory;
                    dir.name = targetDir;
                    currentDirectory.subDirectories.Add(dir);
                }
                currentDirectory = currentDirectory.GetDirectory(targetDir);
            }
        }
    }
    else
    {
        string[] elem = cmd.Split(' ');
        long size = 0;
        if (long.TryParse(elem[0], out size))
        {
            DirFile file = new DirFile(elem[1], size);
            currentDirectory.files.Add(file);
        }
        else
        {
            Directory dir = new Directory();
            dir.parent = currentDirectory;
            dir.name = elem[1];
            currentDirectory.subDirectories.Add(dir);
        }
    }
}
long sizeSum = root.GetSizeSum(100000);
Console.WriteLine("Part one : sum = {0}", sizeSum);

long rootDirSize = root.GetSize();
long missingSize = 30000000 - (70000000 - rootDirSize);
if (missingSize < 0)
{
    Console.WriteLine("Enough free space remaining");
    return;
}
long sizeToDelete = root.GetMinDirFreeSize(missingSize);
Console.WriteLine("Part two : size = {0}", sizeToDelete);





public class Directory
{
    public string name;
    public List<Directory> subDirectories;
    public List<DirFile> files;
    public Directory? parent;

    public Directory()
    {
        subDirectories = new List<Directory>();
        files = new List<DirFile>();
        parent = null;
        name = string.Empty;
    }

    public Directory? GetDirectory(string name)
    {
        if (this.name == name)
        {
            return this;
        }
        else
        {
            for (int i = 0; i < subDirectories.Count; i++)
            {
                if (subDirectories[i].name == name)
                {
                    return subDirectories[i];
                }
            }
        }
        return null;
    }

    public long GetSize()
    {
        long size = 0;
        foreach (var f in files)
        {
            size += f.size;
        }
        foreach (var d in subDirectories)
        {
            size += d.GetSize();
        }
        return size;
    }

    public long GetSizeSum(long maxSize)
    {
        long size = GetSize();
        if (size > maxSize)
        {
            size = 0;
        }
        foreach(var d in subDirectories)
        {
            size += d.GetSizeSum(maxSize);
        }
        return size;
    }

    public long GetMinDirFreeSize(long minSizeToFree)
    {
        long size = GetSize();
        foreach (var d in subDirectories)
        {
            long subSize = d.GetMinDirFreeSize(minSizeToFree);
            if (subSize < size && subSize >= minSizeToFree)
            {
                size = subSize;
            }
        }
        //Si le repertoire est plus petit que la taille à libérer, on le considère comme un répertoire de taille maximale
        //Pour qu'obligatoirement un autre répertoire soit considéré
        if (size <  minSizeToFree)
        {
            size = long.MaxValue;
        }
        return size;
    } 
}


public struct DirFile
{
    public string name;
    public long size;

    public DirFile(string name, long size)
    {
        this.name = name;
        this.size = size;
    }
}