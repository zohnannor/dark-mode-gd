# ChatGPT Gaming

Param(
    [Parameter(Mandatory=$true)]
    [string]$version,

    [switch]$delete
)

# Function to create a git tag and push it to origin
function CreateAndPushTag {
    $tagName = "v$version"
    $tagMessage = $tagName

    # Create the tag locally
    git tag -a $tagName -m $tagMessage

    # Push the tag to origin
    git push origin $tagName
}

# Function to delete a git tag locally and on remote
function DeleteTag {
    $tagName = "v$version"

    # Delete the tag locally
    git tag -d $tagName

    # Delete the tag on remote
    git push origin --delete $tagName
}

# Main script logic
if ($delete) {
    # Delete the tag
    DeleteTag
    Write-Output "Tag $version deleted locally and on remote."
} else {
    # Create and push the tag
    CreateAndPushTag
    Write-Output "Tag $version created and pushed to remote."
}
